#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// There are n kids with candies. You are given an integer array candies, where
// each candies[i] represents the number of candies the ith kid has, and an
// integer extraCandies, denoting the number of extra candies that you have.
//
// Return a boolean array result of length n, where result[i] is true if, after
// giving the ith kid all the extraCandies, they will have the greatest number
// of candies among all the kids, or false otherwise.
//
// Note that multiple kids can have the greatest number of candies.
bool *kidsWithCandies(int *candies, int candiesSize, int extraCandies,
                      int *returnSize) {

  bool *out = (bool *)malloc(candiesSize * sizeof(bool));
  int max = 0;

  memset(out, 0, sizeof(bool) * candiesSize);
  *returnSize = candiesSize;

  for (int i = 0; i < candiesSize; ++i) {
    if (candies[i] > max)
      max = candies[i];
  }

  for (int i = 0; i < candiesSize; ++i) {
    if ((candies[i] + extraCandies) >= max)
      out[i] = true;
  }

  return out;
}

int main(void) {
  int candies[] = {2, 3, 5, 1, 3};
  int extra = 3;
  int rsize = 0;

  bool *r =
      kidsWithCandies(candies, sizeof(candies) / sizeof(int), extra, &rsize);

  printf("[");
  for (int i = 0; i < rsize; ++i) {
    if (r[i]) {
      printf("true");
    } else {
      printf("false");
    }
    printf(",");
  }
  printf("]");
  free(r);
}
