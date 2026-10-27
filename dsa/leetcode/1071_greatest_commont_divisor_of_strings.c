#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strnmul(const char *pref, const int end, const int n) {

  char *out = (char *)malloc(sizeof(char) * (n * end + 1));

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < end; j++) {
      out[j + (end * i)] = pref[j];
    }
  }
  out[n * end] = '\0';

  return out;
}

char *gcdOfStrings(char *str1, char *str2) {
  int l1 = strlen(str1);
  int l2 = strlen(str2);
  int n1_reps, n2_reps;
  char *out = "";
  char *c1 = NULL, *c2 = NULL;

  int m = l1 < l2 ? l1 : l2;
  char *prefix = l1 < l2 ? str1 : str2;

  for (int i = m; i > 0; --i) {
    if (l1 % i != 0 || l2 % i != 0)
      continue;
    n1_reps = l1 / i;
    n2_reps = l2 / i;

    c1 = strnmul(prefix, i, n1_reps);
    c2 = strnmul(prefix, i, n2_reps);

    if (!strcmp(str1, c1) && !strcmp(str2, c2)) {
      out = (char *)malloc(sizeof(char) * (i + 1));
      strncpy(out, prefix, i);
      out[i] = '\0';
      break;
    }
  }

  if (c1)
    free(c1);
  if (c2)
    free(c2);

  return out;
}

int main(void) {
  char str1[] = "ABABAB";
  char str2[] = "ABAB";
  char *res = gcdOfStrings(str1, str2);

  printf("%s", res);
  free(res);
}
