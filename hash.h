#include <stdio.h>

struct hlist_node {
    struct hlist_node *next, **pprev;
};

struct hlist_head {
    struct hlist_node *first;
};

typedef struct {
    char type;  // 'i' for integer hashing, 's' for string hashing
    int bits;
    struct hlist_head *ht;
} map_t;

struct hash_key {
    int key;
    void *data;
    struct hlist_node node;
};

struct hash_skey {
    char *skey;
    void *data;
    struct hlist_node node;
};

#define GOLDEN_RATIO_32 0x61C88647  // 1640531526.5
#define GOLDEN_RATIO_64 0x61C8864680B583EBull

static inline unsigned int ihash(unsigned int val, unsigned int bits)
{
    return (val * GOLDEN_RATIO_32) >> (32 - bits);
}

static inline unsigned int shash(const char *str, unsigned int bits)
{
    if (!str)
        return 0;

    unsigned int ret = 1, len = strlen(str);
    for (int i = 0; i < len; i++)
        ret = (ret * str[i] * GOLDEN_RATIO_32) >> (32 - bits);

    return ret;
}

#define hash(val, bits) \
    _Generic((val), \
    unsigned int: ihash, \
    int: ihash, \
    const char *: shash, \
    char *: shash \
)(val, bits)
