#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SECONDS 10000
#define NINO 1000000000

struct timespec time_diff(struct timespec start, struct timespec end);
int main(int argc, char *argv[]) {
    // hash major code
    struct timespec start, end, diff;
    clock_gettime(CLOCK_MONOTONIC, &start);

    FILE *fout;

    if (argc < 3) {
        printf("should input bits and size.\n");
        exit(0);
    }

    if (!(fout = fopen("generate.dat", "w"))) {
        printf("file open error!\n");
        exit(0);
    }

    int bits = atoi(argv[1]), size = atoi(argv[2]);
    map_t* hmap = map_init(bits);

    if (!hmap) {
        printf("hash map malloc error!\n");
        exit(0);
    }

    for (int i = 0; i < size; i++) {
        fprintf(fout, "%d\n", hash(i, bits));
        map_add(hmap, i, NULL);
    }

    int ccnt = 0;
    for (int i = 0; i <  MAP_HASH_SIZE(bits); i++) {
        ccnt += (hmap->ht)[i].cnt ? (hmap->ht)[i].cnt - 1 : 0;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    diff = time_diff(start, end);
    printf("execution time = %lu.%lu sec\n", diff.tv_sec, diff.tv_nsec);
    printf("collision rate = %f\n", (float)ccnt / size);

    fclose(fout);

    return 0;
}

struct timespec time_diff(struct timespec start, struct timespec end) {
  struct timespec temp;
  if ((end.tv_nsec - start.tv_nsec) > end.tv_nsec) {
    temp.tv_sec = end.tv_sec - start.tv_sec - 1;
    temp.tv_nsec = NINO + end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp;
}