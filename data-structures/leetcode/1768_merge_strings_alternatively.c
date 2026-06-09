#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// You are given two strings word1 and word2. Merge the strings by adding
// letters in alternating order, starting with word1. If a string is longer than
// the other, append the additional letters onto the end of the merged string.
//
// Return the merged string.
char *mergeAlternately(char *word1, char *word2) {

  int l1 = strlen(word1);
  int l2 = strlen(word2);
  char *result = (char *)malloc((1 + l1 + l2) * sizeof(char));
  char *p = NULL;

  int i = 0, j = 0;

  int min = l1 < l2 ? l1 : l2;
  int max = l1 < l2 ? l2 : l1;
  p = min == l1 ? word2 : word1;

  for (i = 0; i < min; ++i) {
    j = i * 2;
    result[j] = word1[i];
    result[j + 1] = word2[i];
  }

  j = min * 2;

  for (i = min; i < max; ++i) {
    result[j++] = p[i];
  }

  result[j] = '\0';

  return result;
}

int main(void) {
  char *word1 = "abcd";
  char *word2 = "pqr";
  char *res = mergeAlternately(word1, word2);

  printf("%s\n", res);

  free(res);
  return 0;
}
