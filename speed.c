#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

#define NINO 1000000000

struct timespec time_diff(struct timespec start, struct timespec end);

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("should input bits and size.\n");
        exit(0);
    }

    int bits = atoi(argv[1]), size = atoi(argv[2]);
    int _;

    struct timespec start, end, diff;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < size; i++) {
        _ = hash(i, bits);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    diff = time_diff(start, end);
    printf("execution time = %lu.%lu sec\n", diff.tv_sec, diff.tv_nsec);

    return 0;
}

struct timespec time_diff(struct timespec start, struct timespec end) {
  struct timespec temp;
  if ((end.tv_nsec-start.tv_nsec) < 0) {
    temp.tv_sec = end.tv_sec - start.tv_sec - 1;
    temp.tv_nsec = NINO + end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp;
}