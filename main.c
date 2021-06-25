#include <stdio.h>
#include <stdlib.h>
#include "hash.h"


int *twoSum(int *nums, int numsSize, int target, int *returnSize);

int main(int argc, char * argv []) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int *ret = (int *)malloc(sizeof(int) * 2);
    int ret_size;

    ret = twoSum(arr, 10, 18, &ret_size);

    printf("return array : ");
    for (int i = 0; i < ret_size; i++)
        printf("%d ", ret[i]);
    printf("\nprocess complete.\n");
    return 0;
}


int *twoSum(int *nums, int numsSize, int target, int *returnSize)
{
    map_t *map = map_init(10);
    *returnSize = 0;
    int *ret = (int *)malloc(sizeof(int) * 2);
    if (!ret)
        goto bail;

    for (int i = 0; i < numsSize; i++) {
        int *p = (int *)map_get(map, target - nums[i]);
        if (p) { /* found */
            ret[0] = i, ret[1] = *p;
            *returnSize = 2;
            break;
        }

        p = (int *)malloc(sizeof(int));
        *p = i;
        map_add(map, nums[i], p);
    }

bail:
    map_deinit(map);
    return ret;
}