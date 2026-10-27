#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// You have a long flowerbed in which some of the plots are planted, and some
// are not. However, flowers cannot be planted in adjacent plots.
//
// Given an integer array flowerbed containing 0's and 1's, where 0 means empty
// and 1 means not empty, and an integer n, return true if n new flowers can be
// planted in the flowerbed without violating the no-adjacent-flowers rule and
// false otherwise.

bool canPlaceFlowers(int *flowerbed, int flowerbedSize, int n) {
  for (int i = 0; i < flowerbedSize; ++i) {
    if (flowerbed[i] == 0) {
      // is first?
      if (i == 0) {
        if (flowerbedSize > 1 && flowerbed[i + 1] == 0) {
          n--;
          flowerbed[i] = 1;
        } else if (flowerbedSize == 1) {
          n--;
          flowerbed[i] = 1;
        }
        continue;
      }

      // is last?
      if (i == (flowerbedSize - 1)) {
        if (flowerbed[i - 1] == 0) {
          n--;
          flowerbed[i] = 1;
        }
        continue;
      }

      // no first and last element
      if (i > 0 && i < (flowerbedSize - 1)) {
        if (flowerbed[i - 1] == 0 && flowerbed[i + 1] == 0) {
          n--;
          flowerbed[i] = 1;
        }
      }
    }
  }
  return n <= 0;
}

int main(void) {
  int flowerbed[] = {0, 0, 1, 0, 0};
  int n = 1;
  bool r = canPlaceFlowers(flowerbed, sizeof(flowerbed) / sizeof(int), n);

  printf("%d\n", r);

  for (int i = 0; i < 5; ++i) {
    printf("%d,", flowerbed[i]);
  }
}
