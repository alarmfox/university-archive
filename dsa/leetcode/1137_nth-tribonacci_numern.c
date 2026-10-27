// The Tribonacci sequence Tn is defined as follows:
//
// T0 = 0, T1 = 1, T2 = 1, and Tn+3 = Tn + Tn+1 + Tn+2 for n >= 0.
//
// Given n, return the value of Tn.

#include <stdio.h>

int tribonacci(int n) {
  int dp[] = {0, 1, 1}, k;

  if (n < 3)
    return dp[n];

  for (int i = 3; i <= n; ++i) {
    k = dp[0] + dp[1] + dp[2];
    dp[0] = dp[1];
    dp[1] = dp[2];
    dp[2] = k;
  }

  return dp[2];
}

int main(void) {
  printf("%d\n", tribonacci(25));
  return 0;
}
