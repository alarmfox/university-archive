#include <stdbool.h>
#include <stdio.h>
#include <string.h>
// Given two strings s and t, return true if s is a subsequence of t, or false
// otherwise.
//
// A subsequence of a string is a new string that is formed from the original
// string by deleting some (can be none) of the characters without disturbing
// the relative positions of the remaining characters. (i.e., "ace" is a
// subsequence of "abcde" while "aec" is not).
/*
 * ^
 * a b c        i = 0
 * ^
 * a h b g d c  j = 0
 *
 *   ^
 * a b c         i = 1
 *   ^
 * a h b g d c   j = 1
 *     ^
 * a h b g d c   j = 2

       ^
 * a b c         i = 2
 *       ^
 * a h b g d c   j = 3
       ^
 * a b c         i = 2
 *       ^
 * a h b g d c   j = 3
 *
 *
 */
bool isSubsequence(char *s, char *t) {
  size_t l1 = strlen(s);
  size_t l2 = strlen(t);
  size_t i = 0;
  size_t j = 0;

  while (i < l1 && j < l2) {
    if (s[i] == t[j++])
      i++;
  }

  return i == l1;
}

int main(void) {
  char s[] = "abc";
  char t[] = "ahbgdc";

  printf("%d\n", isSubsequence(s, t));

  strcpy(s, "axc");
  strcpy(t, "ahbgdc");
  printf("%d\n", isSubsequence(s, t));

  return 0;
}
