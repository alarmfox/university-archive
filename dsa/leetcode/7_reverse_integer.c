#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int reverse(int x) {
    int rev = 0;
    while (x != 0) {
        int pop = x % 10;
        x /= 10;

        // 1. Check for Positive Overflow
        // If rev > 214748364, then rev * 10 will definitely overflow.
        // If rev == 214748364, it only overflows if the next digit is > 7.
        if (rev > INT_MAX / 10 || (rev == INT_MAX / 10 && pop > 7)) {
            return 0;
        }

        // 2. Check for Negative Overflow
        // If rev < -214748364, then rev * 10 will definitely overflow.
        // If rev == -214748364, it only overflows if the next digit is < -8.
        if (rev < INT_MIN / 10 || (rev == INT_MIN / 10 && pop < -8)) {
            return 0;
        }

        rev = rev * 10 + pop;
    }
    return rev;
}

int main(void) {
  printf("%d -> %d\n", 1534236469, reverse(1534236469));
  printf("%d -> %d\n", 123, reverse(123));
}
