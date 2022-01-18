#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hash.h"

// util
#define BUILD_BUG_ON_ZERO(e) ((int)(sizeof(struct { int:(-!!(e)); })))
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#define WRITE_ONCE(var, val) \
    (*((volatile typeof(val) *)(&(var))) = (val))

// for size counting
#define MAP_HASH_SIZE(bits) (1 << bits)
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr)
#define HASH_SIZE(name) ARRAY_SIZE(name)
#define HASH_BITS(name) ilog2(HASH_SIZE(name))

// for iteration
#define HLIST_FOR_EACH(tmp, head) \
    for (struct hlist_node *tmp = head; tmp; tmp = tmp->next)
#define HMAP_FOR_EACH(heah, map, index) \
    int index = 0;\
    for (struct hlist_node *heah = (map->ht)[index].first; index < MAP_HASH_SIZE(map->bits); heah = (map->ht)[++index].first)

// check type
#define TYPE_CONSISTENT(map, t) \
    (map && map->type == t) ? 1 : 0

#define container_of(ptr, type, member)                            \
    __extension__({                                                \
        const __typeof__(((type *) 0)->member) *__pmember = (ptr); \
        (type *) ((char *) __pmember - offsetof(type, member));    \
    })



struct hmap_info {
    int key_cnt;
    float load_factor;
    float empty_rate;
    float exactly_one_rate;
    float more_than_one_rate;
    float collision_rate;
};

map_t *map_init(int bits, char type) {
    map_t *map = (map_t *)malloc(sizeof(map_t));
    if (!map)
        return NULL;

    map->bits = bits;
    map->type = type; // assign type (int or char *)
    map->ht = (struct hlist_head*)malloc(sizeof(struct hlist_head) * MAP_HASH_SIZE(map->bits));
    if (map->ht) {
        for (int i = 0; i < MAP_HASH_SIZE(map->bits); i++){
            (map->ht)[i].first = NULL;
        }
    } else {
        free(map);
        map = NULL;
    }
    return map;
}

static struct hash_key *find_ikey(map_t *map, int key) {
    assert(TYPE_CONSISTENT(map, 'i'));
    struct hlist_head *head = &(map->ht)[hash(key, map->bits)]; //change
    for (struct hlist_node *p = head->first; p; p = p->next) {
        struct hash_key *kn = (struct hash_key *)container_of(p, struct hash_key, node);
        if (kn->key == key)
            return kn;
    }
    return NULL;
}

static struct hash_skey *find_skey(map_t *map, char * skey) {
    assert(TYPE_CONSISTENT(map, 's'));
    struct hlist_head *head = &(map->ht)[hash(skey, map->bits)]; //change
    for (struct hlist_node *p = head->first; p; p = p->next) {
        struct hash_skey *kn = container_of(p, struct hash_skey, node);
        if (strcmp(kn->skey, skey) == 0)
            return kn;
    }
    return NULL;
}

#define find_key(map, key)  \
    _Generic((key), \
        int: find_ikey, \
        char*: find_skey\
    )(map, key)\


void *map_geti(map_t *map, int key)
{
    assert(TYPE_CONSISTENT(map, 'i'));

    struct hash_key *kn = find_key(map, key);
    return kn ? kn->data : NULL;
}

void *map_gets(map_t *map, char * skey)
{
    assert(TYPE_CONSISTENT(map, 's'));

    struct hash_skey *kn = find_key(map, skey);
    return kn ? kn->data : NULL;
}

#define map_get(map, key) \
    _Generic((key), \
        int: map_geti, \
        char*: map_gets\
    )(map, key)\


void map_addi(map_t *map, int key, void *data)
{
    assert(TYPE_CONSISTENT(map, 'i'));

    struct hash_key *kn = find_key(map, key);
    if (kn)
        return;

    kn = (struct hash_key*)malloc(sizeof(struct hash_key));
    if (!kn)
        return; 

    kn->key = key, kn->data = data;
    struct hlist_head *h = &map->ht[hash(key, map->bits)];
    struct hlist_node *n = &kn->node, *first = h->first;
    n->next = first;
    
    if (first)
        first->pprev = &n->next;
    h->first = n;
    n->pprev = &h->first;
}

void map_adds(map_t *map, char *skey, void *data)
{
    assert(TYPE_CONSISTENT(map, 's'));
    struct hash_skey *kn = find_key(map, skey);
    if (kn)
        return;

    kn = (struct hash_skey*)malloc(sizeof(struct hash_skey));
    if (!kn) // add
        return;
    
    size_t len = strlen(skey) + 1;
    kn->skey = (char *)malloc(sizeof(char) * len);
    if (!kn->skey) {
        free(kn);
        return;
    }

    strncpy(kn->skey, skey, len);
    kn->skey[len-1] = '\0';
    kn->data = data;
    struct hlist_head *h = &map->ht[hash(skey, map->bits)];
    struct hlist_node *n = &kn->node, *first = h->first;
    n->next = first;
    
    if (first)
        first->pprev = &n->next;
    h->first = n;
    n->pprev = &h->first;
}

#define map_add(map, key, data)  \
    _Generic((key), \
        int: map_addi, \
        char*: map_adds\
    )(map, key, data)\


void map_deli(map_t *map, int key) {
    assert(TYPE_CONSISTENT(map, 'i'));
    struct hash_key *kn = find_key(map, key);
    if (!kn)
        return;
    
    struct hlist_node **pprev = kn->node.pprev;
    struct hlist_node *next = kn->node.next;
    
    *pprev = next;
    if (next) 
        next->pprev = pprev;
    
    if (kn->data)
        free(kn->data);
    free(kn);
}

void map_dels(map_t *map, char *skey) {
    assert(TYPE_CONSISTENT(map, 's'));
    struct hash_skey *kn = find_key(map, skey);
    if (!kn)
        return;
    
    struct hlist_node **pprev = kn->node.pprev;
    struct hlist_node *next = kn->node.next;
    
    *pprev = next;
    if (next) 
        next->pprev = pprev;
    
    if (kn->data)
        free(kn->data);
    free(kn);
}

#define map_del(map, key)  \
    _Generic((key), \
        int: map_deli, \
        char*: map_dels\
    )(map, key)\



void map_deinit(map_t *map)
{
    assert(TYPE_CONSISTENT(map, 'i'));
    if (!map)
        return;

    for (int i = 0; i < MAP_HASH_SIZE(map->bits); i++) {
        struct hlist_head *head = &map->ht[i];
        for (struct hlist_node *p = head->first; p;) {
            struct hash_key *kn = container_of(p, struct hash_key, node);
            struct hlist_node *n = p;
            p = p->next;

            // modify

            struct hlist_node *next = n->next, **pprev = n->pprev;
            *pprev = next;
            if (next)
                next->pprev = pprev;
            n->next = NULL, n->pprev = NULL;

            // modify
            if (kn->data)
                free(kn->data);
            free(kn);
        }
    }
    free(map);
}

void map_deinits(map_t *map)
{
    assert(TYPE_CONSISTENT(map, 's'));
    if (!map)
        return;

    for (int i = 0; i < MAP_HASH_SIZE(map->bits); i++) {
        struct hlist_head *head = &map->ht[i];
        for (struct hlist_node *p = head->first; p;) {
            struct hash_skey *kn = container_of(p, struct hash_skey, node);
            struct hlist_node *n = p;
            p = p->next;

            // modify

            struct hlist_node *next = n->next, **pprev = n->pprev;
            *pprev = next;
            if (next)
                next->pprev = pprev;
            n->next = NULL, n->pprev = NULL;

            // modify
            if (kn->data)
                free(kn->data);
            free(kn);
        }
    }
    free(map);
}





/* 
 * show the information of :
 * 1. load factor
 * 2. bucket non-used ratio
 * 3. bucket exactly-one ratio
 * 4. bucket more-than-one ratios
 * 5. collision rate
 */
void map_log_info(map_t *map) {
    if (!map) {
        printf("hash map log info error: hash map is empty.\n");
        return;
    }
    int b_size = MAP_HASH_SIZE(map->bits);
    int key_cnt = 0, non_cnt = 0, one_cnt = 0, mto_cnt = 0, col_cnt = 0;

    HMAP_FOR_EACH(head, map, i){
        int b_cnt = 0;
        HLIST_FOR_EACH(tmp, head)
            b_cnt++;
        key_cnt += b_cnt;
        non_cnt += !b_cnt ? 1 : 0;
        one_cnt += b_cnt == 1 ? 1 : 0;
        mto_cnt += b_cnt > 1 ? 1 : 0;
        col_cnt += b_cnt ? (b_cnt - 1) : 0;
    }
    
    printf("============== result ==============\n");
    printf("| num of key = %d\n", key_cnt);
    printf("| load factor = %f\n", (float)key_cnt / b_size);
    printf("| not-used rate = %f\n", (float)non_cnt / b_size);
    printf("| exactly-one rate = %f\n", (float)one_cnt / b_size);
    printf("| more-than-one rate = %f\n", (float)mto_cnt / b_size);
    printf("| collision rate = %f\n", (float)col_cnt / key_cnt);
    printf("====================================\n");
}

/* 
 * get the information of :
 * 1. load factor
 * 2. bucket non-used ratio
 * 3. bucket exactly-one ratio
 * 4. bucket more-than-one ratios
 * 5. collision rate
 */
struct hmap_info* map_get_info(map_t *map) {
    if (!map) {
        printf("hash map log info error: hash map is empty.\n");
        return NULL;
    }

    struct hmap_info *ret = (struct hmap_info *)malloc(sizeof(struct hmap_info));
    if (!ret) {
        printf("hash map log info error: return value malloc error.\n");
        return NULL;
    }

    int b_size = MAP_HASH_SIZE(map->bits);
    int key_cnt = 0, non_cnt = 0, one_cnt = 0, mto_cnt = 0, col_cnt = 0;


    HMAP_FOR_EACH(head, map, i){
        int b_cnt = 0;
        HLIST_FOR_EACH(tmp, head)
            b_cnt++;
        key_cnt += b_cnt;
        non_cnt += !b_cnt ? 1 : 0;
        one_cnt += b_cnt == 1 ? 1 : 0;
        mto_cnt += b_cnt > 1 ? 1 : 0;
        col_cnt += b_cnt ? (b_cnt - 1) : 0;
    }

    ret->key_cnt = key_cnt;
    ret->load_factor = (float)key_cnt / b_size;
    ret->empty_rate = (float)non_cnt / b_size;
    ret->exactly_one_rate = (float)one_cnt / b_size;
    ret->more_than_one_rate = (float)mto_cnt / b_size;
    ret->collision_rate = (float)col_cnt / key_cnt;

    return ret;
}

