#include <math.h>
#include <stdio.h>

int monotoneIncreasingDigits(int n) {
    char digits[10] = {0};
    int len = 0;

    while(n) {
        digits[len++] = (n % 10) + '0';
        n/= 10;
    }

    int marker = -1;
    for (int i = len - 1; i >= 1; --i) {
        if(digits[i] > digits[i-1]) {
          marker = i - 1;
          digits[i-1] -= 1;
        }
    }

    return n;
}

int main(void) {
  printf("%d\n", monotoneIncreasingDigits(1234));
  printf("%d\n", monotoneIncreasingDigits(332));
  printf("%d\n", monotoneIncreasingDigits(10));
}
