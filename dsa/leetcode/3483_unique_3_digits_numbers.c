#include <stdio.h>
#include <stdlib.h>

int iscontained(int *a, int n, int target) {
    for(size_t i = 0; i < n; ++i) {
        if(a[i] == target)  return 1;
    }

    return 0;
}

int totalNumbers(int* digits, int digitsSize) {
    int seen[1000] = {0};
    int t = 0;

    for(int i = 0; i < digitsSize; ++i) {
        if(digits[i] == 0) continue;
        for(int j = 0; j < digitsSize; ++j) {
            if(i == j) continue;

            for(int k = 0; k < digitsSize; ++k) {
                if (i == k || j == k) continue;

                int n = digits[i] * 100 + digits[j] * 10 + digits[k];

                if(n % 2 == 0 && seen[n] == 0) {
                    seen[n] = 1;
                    t+=1;

                }

            }
        }
    }

    // for(size_t i = 0; i < t; ++i) {
    //     printf("%d\n", combs[i]);
    // }


    return t;
}

int main(void) {

    printf("%d\n",totalNumbers((int[]) {1, 2, 3, 4}, 4));
    printf("%d\n",totalNumbers((int[]) {0, 2, 2},3));
}
