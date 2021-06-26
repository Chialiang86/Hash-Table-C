#include "hash.h"
#include <stdio.h>
#include <stdlib.h>

struct timespec time_diff(struct timespec start, struct timespec end);
int main(int argc, char *argv[]) {
    // hash major code

    if (argc < 3) {
        printf("should input bits and basic_size.\n");
        exit(0);
    }

    int bits = atoi(argv[1]), basic_size = atoi(argv[2]), iteration_num = argc > 3 ? atoi(argv[3]) : 6;
    printf("hash table size = %d, basic num of key = %d, iteration num = %d .\n", MAP_HASH_SIZE(bits), basic_size, iteration_num);

    map_t* hmap = map_init(bits);

    if (!hmap) {
        printf("hash map malloc error!\n");
        exit(0);
    }

    FILE *fout_scal;
    if (!(fout_scal = fopen("dat/scalability.dat", "w"))) {
      printf("file open error!\n");
      exit(0);
    }

    for (int iter = 1; iter <= iteration_num; iter++) {

      printf("iteration %d ...\n", iter);
      
      FILE *fout_hash, *fout_dist;
      char *f_hash = (char *)malloc(sizeof(char) * 32), *f_dist = (char *)malloc(sizeof(char) * 32);
      if (!f_hash || !f_dist) {
        printf("filename malloc error!\n");
        exit(0);
      }
      int size = iter * basic_size;

      sprintf(f_hash, "dat/generate_%d.dat", size);
      sprintf(f_dist, "dat/distribution_%d.dat", size);

      if (!(fout_hash = fopen(f_hash, "w")) || !(fout_dist = fopen(f_dist, "w"))) {
          printf("file open error!\n");
          exit(0);
      }

      for (int i = 0; i < size; i++) {
          fprintf(fout_hash, "%d\n", hash(i, bits));
          map_add(hmap, i, NULL);
      }

      int ncnt = 0, ucnt = 0, ccnt = 0;
      for (int i = 0; i <  MAP_HASH_SIZE(bits); i++) {
          int tmp = (hmap->ht)[i].cnt;
          ncnt += !tmp ? 1 : 0; // count empty : increment if this bucket is empty 
          ucnt += tmp == 1 ? 1 : 0; // count unique : increment if this bucket contains exactly one key
          ccnt += tmp ? tmp - 1 : 0; // count collision : if this bucket contains more than one key, then add ("num of key in this bucket" minus 1)
          fprintf(fout_dist, "%d\n", tmp);
      }

      // count ratio 
      float load_factor = (float)size / MAP_HASH_SIZE(bits);  // n divide by b, n is the num of keys, b is the num of buckets
      float not_used_rate = (float)ncnt / MAP_HASH_SIZE(bits) * 100;
      float exactly_one_rate = (float)ucnt / MAP_HASH_SIZE(bits) * 100;
      float more_than_one_rate = (float)(MAP_HASH_SIZE(bits) - ncnt - ucnt) / MAP_HASH_SIZE(bits) * 100;
      float collision_rate = (float)ccnt / size * 100;

      fprintf(fout_scal, "%.2f %f %f %f %f\n", load_factor, not_used_rate, exactly_one_rate, more_than_one_rate, collision_rate);

      printf("============== result ==============\n");
      printf("num of key = %d\n", size);
      printf("load factor = %f\n", load_factor);
      printf("not-used rate = %f%%\n", not_used_rate);
      printf("exactly-one rate = %f%%\n", exactly_one_rate);
      printf("more-than-one rate = %f%%\n", more_than_one_rate);
      printf("collision rate = %f%%\n", collision_rate);
      printf("====================================\n");

      fclose(fout_hash);
      fclose(fout_dist);
      free(f_hash); 
      free(f_dist);
    }
    fclose(fout_scal);
    return 0;
}