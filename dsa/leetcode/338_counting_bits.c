// Given an integer n, return an array ans of length n + 1 such that for each i
// (0 <= i <= n), ans[i] is the number of 1's in the binary representation of i.
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *countBits(int n, int *returnSize) {
  *returnSize = n + 1;

  int *ans = malloc(sizeof(int) * (n + 1));

  for (int i = 0; i <= n; ++i) {
    int k = i;
    ans[i] = 0;
    while (k) {

      ans[i] += (k & 0x1) == 0 ? 0 : 1;

      k = k >> 1;
    }
  }

  return ans;
}

int main(void) {
  int n = 5, outSize;
  int *ans = countBits(n, &outSize);

  for (int i = 0; i < outSize; ++i)
    printf("%d ", ans[i]);
  printf("\n");

  return 0;
}
