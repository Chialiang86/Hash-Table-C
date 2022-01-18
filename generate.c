#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct timespec time_diff(struct timespec start, struct timespec end);
int main(int argc, char *argv[]) {
    // hash major code

    if (argc < 3) {
        printf("should input bits and basic_size.\n");
        exit(0);
    }

    int bits = atoi(argv[1]), basic_size = atoi(argv[2]), iteration_num = argc > 3 ? atoi(argv[3]) : 1;
    printf("hash table size = %d, basic num of key = %d, iteration num = %d .\n", MAP_HASH_SIZE(bits), basic_size, iteration_num);

    FILE *fout_scal;
    if (!(fout_scal = fopen("dat/scalability.dat", "w"))) {
      printf("file open error!\n");
      exit(0);
    }

    srand(time(NULL));

    int shift_num = 12;

    for (int iter = 1; iter <= iteration_num; iter++) {

      map_t* hmap = map_init(bits, 's');
      if (!hmap) {
          printf("hash map malloc error!\n");
          exit(0);
      }
      
      FILE *fout_hash;
      char *f_hash = (char *)malloc(sizeof(char) * 32);
      if (!f_hash) {
        printf("filename malloc error!\n");
        exit(0);
      }
      int size = iter * basic_size;

      sprintf(f_hash, "dat/generate_%d.dat", size);

      if (!(fout_hash = fopen(f_hash, "w"))) {
          printf("file open error!\n");
          exit(0);
      }

      printf("iteration %d ...\n", iter);
      for (int i = 0; i < size; i++) {
          int s_size = 3 + rand() % 27;
          char *s = (char *)malloc(sizeof(char) * (s_size + 1));
          if (s) {
              for (int si = 0; si < s_size; si++)
                  s[si] = (char)(rand() % 92 + 33); 
              s[s_size] = '\0';
              printf("key : \"%s\", value : %d\n", s, hash(s, bits));
              fprintf(fout_hash, "%d\n", hash(s, bits));
              map_add(hmap, s, NULL);
              free(s);
          }
      }

      // count ratio
      struct hmap_info * info = map_get_info(hmap);
      if (info) {
        fprintf(fout_scal, "%.2f %f %f %f %f\n", info->load_factor, info->empty_rate, info->exactly_one_rate, info->more_than_one_rate, info->collision_rate);
        printf("============== result ==============\n");
        printf("| num of key = %d\n", info->key_cnt);
        printf("| load factor = %f\n", info->load_factor);
        printf("| not-used rate = %f%%\n", info->empty_rate);
        printf("| exactly-one rate = %f%%\n", info->exactly_one_rate);
        printf("| more-than-one rate = %f%%\n", info->more_than_one_rate);
        printf("| collision rate = %f%%\n", info->collision_rate);
        printf("====================================\n");
      }

      fclose(fout_hash);
      free(f_hash);
      map_deinits(hmap);
    }
    
    fclose(fout_scal);
    return 0;
}