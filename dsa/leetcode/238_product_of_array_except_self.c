#include <stdio.h>
#include <stdlib.h>

// Given an integer array nums, return an array answer such that answer[i] is
// equal to the product of all the elements of nums except nums[i].
//
// The product of any prefix or suffix of nums is guaranteed to fit in a 32-bit
// integer.
//
// You must write an algorithm that runs in O(n) time and without using the
// division operation.

// int *productExceptSelf(int *nums, int numsSize, int *returnSize) {
//   int *out = (int *)malloc(numsSize * sizeof(int));
//   int prefix[MAX_SIZE], postfix[MAX_SIZE];
//   *returnSize = numsSize;
//
//   int acc = 1;
//   for (int i = 0; i < numsSize; ++i) {
//     acc *= nums[i];
//     prefix[i] = acc;
//   }
//   acc = 1;
//   for (int i = numsSize - 1; i >= 0; --i) {
//     acc *= nums[i];
//     postfix[i] = acc;
//   }
//
//   for (int i = 0; i < numsSize; ++i) {
//     int pref = i == 0 ? 1 : prefix[i - 1];
//     int post = i == (numsSize - 1) ? 1 : postfix[i + 1];
//     out[i] = pref * post;
//   }
//   return out;
// }

int *productExceptSelf(int *nums, int numsSize, int *returnSize) {
  int *out = malloc(numsSize * sizeof(int));
  *returnSize = numsSize;
  int acc = 1;

  for (int i = 0; i < numsSize; ++i) {
    out[i] = acc;
    acc *= nums[i];
  }
  acc = 1;
  for (int i = numsSize - 1; i >= 0; i--) {
    out[i] *= acc;
    acc *= nums[i];
  }
  return out;
}

int main(void) {
  int nums[] = {1, 2, 3, 4};
  int rsize = 0;
  int *res = productExceptSelf(nums, sizeof(nums) / sizeof(int), &rsize);

  for (int i = 0; i < rsize; ++i) {
    printf("%d, ", res[i]);
  }

  free(res);
}
