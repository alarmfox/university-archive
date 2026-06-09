#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Given an input string s, reverse the order of the words.
//
// A word is defined as a sequence of non-space characters. The words in s will
// be separated by at least one space.
//
// Return a string of the words in reverse order concatenated by a single space.
//
// Note that s may contain leading or trailing spaces or multiple spaces between
// two words. The returned string should only have a single space separating the
// words. Do not include any extra spaces.

char *reverseWords(char *s) {
  int l = strlen(s);
  char *out = (char *)malloc(sizeof(char) * (l + 1));
  int end = 0, k = 0;

  do {
    while (l > 0 && (s[l] == ' ' || s[l] == '\0'))
      l--;
    end = l;
    while (l >= 0 && s[l] != ' ')
      l--;

    if (end >= l + 1) {
      for (int i = l + 1; i <= end; ++i) {
        out[k++] = s[i];
      }
      out[k++] = ' ';
    }

  } while (l > 0);

  if (k > 0 && out[k - 1] == ' ')
    k--;

  out[k] = '\0';
  return out;
}

int main(void) {
  char s[] = "the sky is blue";

  char *r = reverseWords(s);

  printf("%s", r);

  free(r);
}
