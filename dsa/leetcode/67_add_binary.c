#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* addBinary(char* a, char* b) {
    int i = strlen(a) - 1;
    int j = strlen(b) - 1;
    int max = (i < j ? j : i) + 2;
    char *res = malloc(max + 1);
    int k = 0;
    int carry = 0;

    memset(res, 0, max + 1);

    while (i >= 0 || j >= 0 || carry) {
       int an = i >= 0 ? (a[i--] - '0') : 0;
       int bn = j >= 0 ? (b[j--] - '0') : 0;
       int sum = an ^ bn ^ carry;
       res[k++] = (char) (sum + '0');
       carry = (an + bn + carry) > 1 ? 1 : 0;
    }

    /* Reverse the string */
    for(int left = 0, right = k - 1; left < right; left++, right--) {
        char t = res[left];
        res[left] = res[right];
        res[right] = t;
    }

    return res;
}

int main(void) {
  char a[] = "11";
  char b[] = "1";

  printf("%s\n", addBinary(a, b));
}
