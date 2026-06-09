#include <stdio.h>

// Given a non-empty array of integers nums, every element appears twice except
// for one. Find that single one.
//
// You must implement a solution with a linear runtime complexity and use only
// constant extra space.

// a^a = 0
// a ^ 0 = a
int singleNumber(int *nums, int numsSize) {

  int xor = 0;
  for (int i = 0; i < numsSize; ++i) {
    xor ^= nums[i];
  }

  return xor;
}

int main(void) {
  int nums[] = {2, 2, 1};
  int n = sizeof(nums) / sizeof(nums[0]);

  printf("%d\n", singleNumber(nums, n));
  return 0;
}
