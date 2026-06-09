// You are given a string s, which contains stars *.
//
// In one operation, you can:
//
//     Choose a star in s.
//     Remove the closest non-star character to its left, as well as remove the
//     star itself.
//
// Return the string after all stars have been removed.
//
// Note:
//
//     The input will be generated such that the operation is always possible.
//     It can be shown that the resulting string will always be unique.

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *removeStars(char *s) {
  size_t i = 0, k = 0;
  size_t len = strlen(s);
  char *res = malloc(len + 1);

  while (i < len) {
    if (s[i] != '*') {
      res[k++] = s[i];
    } else {
      k = k > 0 ? k - 1 : k;
    }
    ++i;
  }

  res[k] = 0;

  strcpy(s, res);
  free(res);
  res = NULL;

  return s;
}

int main(void) {
  char s[] = "leet**cod*e";
  printf("%s -> %s", s, removeStars(s));
}
