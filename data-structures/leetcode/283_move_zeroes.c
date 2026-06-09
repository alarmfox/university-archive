#include <stdio.h>
// Given an integer array nums, move all 0's to the end of it while maintaining
// the relative order of the non-zero elements.
//
// Note that you must do this in-place without making a copy of the array.
/*
 * ^
 * 0 1 0 3 12
 */
void swap(int *a, int *b) {
  if (!a || !b)
    return;

  int t = *a;
  *a = *b;
  *b = t;
}

void moveZeroes(int *nums, int numsSize) {
  for (int i = 0; i < numsSize; ++i) {
    if (nums[i] != 0)
      continue;
    // find the first non zero element and swap it whit the current zero
    for (int j = i + 1; j < numsSize; ++j) {
      if (nums[j] != 0) {
        swap(&nums[i], &nums[j]);
        break;
      }
    }
  }
}

void moveZeroesOpt(int *nums, int numsSize) {
  int j = 0;
  for (int i = 0; i < numsSize; ++i) {
    // swap with the previous zero encountered
    if (nums[i] != 0) {
      swap(&nums[i], &nums[j]);
      j += 1;
    }
  }
}

int main(void) {
  int nums[] = {0, 1, 0, 3, 12};
  int n = sizeof(nums) / sizeof(nums[0]);

  for (size_t i = 0; i < n; ++i) {
    printf("%d ", nums[i]);
  }
  printf("\n");
  moveZeroesOpt(nums, n);
  for (size_t i = 0; i < n; ++i) {
    printf("%d ", nums[i]);
  }
}
