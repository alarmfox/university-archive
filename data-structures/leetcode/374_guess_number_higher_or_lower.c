// We are playing the Guess Game. The game is as follows:
//
// I pick a number from 1 to n. You have to guess which number I picked (the
// number I picked stays the same throughout the game).
//
// Every time you guess wrong, I will tell you whether the number I picked is
// higher or lower than your guess.
//
// You call a pre-defined API int guess(int num), which returns three possible
// results:
//
//     -1: Your guess is higher than the number I picked (i.e. num > pick).
//     1: Your guess is lower than the number I picked (i.e. num < pick).
//     0: your guess is equal to the number I picked (i.e. num == pick).
//
// Return the number that I picked.

/**
 * Forward declaration of guess API.
 * @param  num   your guess
 * @return 	     -1 if num is higher than the picked number
 *			      1 if num is lower than the picked number
 *               otherwise return 0
 */
/**
 * Forward declaration of guess API.
 * @param  num   your guess
 * @return 	     -1 if num is higher than the picked number
 *			      1 if num is lower than the picked number
 *               otherwise return 0
 */

#include <stdio.h>
static int pick = 6;

int guess(int num) {
  if (num == pick)
    return 0;
  else if (num > pick)
    return -1;
  else
    return 1;
}

int guessNumber(int n) {

  int left = 1, right = n, mid, r;

  while (left < right) {
    mid = (left + right + 1) / 2;

    r = guess(mid);
    if (r == 0)
      return mid;
    if (r == 1)
      left = mid + 1;
    else
      right = mid;
  }
  return 0;
}

int main(void) {
  int a = guessNumber(10);
  printf("%d\n", a);

  return 0;
}
