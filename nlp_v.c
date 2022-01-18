#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hashtable.h"

int main(int argc, char * argv []) {

    FILE *fin, *fout;
    if (!(fin = fopen("dataset/spam.csv", "r")) || !(fout = fopen("dataset/distribution.dat", "w"))) {
        printf("dataset load failed.\n");
        exit(EXIT_FAILURE);
    }

    char line[256];
    char type[8];
    char msg[248];
    int bits = 16;

    map_t *hmap = map_init(bits, 's'); // 49063 / 65536 ~= 0.76
    if (!hmap) {
        printf("hash map malloc error!\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    int max_i = 50000;

    for (int i = 0; i < max_i; i++) {
        int s_size = 3 + rand() % 27;
        char *s = (char *)malloc(sizeof(char) * (s_size + 1));
        if (s) {
            for (int si = 0; si < s_size; si++)
                s[si] = (char)(rand() % 92 + 33); 
            s[s_size] = '\0';
            printf("key : %s, value = %d\n", s, hash(s, bits));
            map_adds(hmap, s, NULL);
            free(s);
        }
    }
 
    // while (fgets(line, sizeof(line), fin) != NULL) {

    //     sscanf(line, "%[^,], %[^\n]", type, msg);

    //     const char *delim = " ";
    //     char *sp;

    //     sp = strtok(msg, delim);
    //     while (sp) {
    //         map_adds(hmap, sp, NULL);
    //         sp = strtok(NULL, delim);
    //     }
    // }

    HMAP_FOR_EACH(head, hmap, i) {
        unsigned int cnt = 0;
        HLIST_FOR_EACH(tmp, head)
            cnt++;
        fprintf(fout, "%x\n", cnt);
    }

    map_log_info(hmap);
    map_deinits(hmap);
    fclose(fin);
    fclose(fout);

    return 0;
}