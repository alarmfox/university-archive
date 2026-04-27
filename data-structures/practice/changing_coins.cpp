#include <bits/stdc++.h>

#define TEST 1

//
// Given an integer array of coins[ ] of size N representing different types of
// currency and an integer sum, The task is to find the number of ways to make
// sum by using different combinations from coins[].
//
// Note: Assume that you have an infinite supply of each type of coin.
//
int min(const int coins[], const int n, const int sum) {
  int dp[n + 1][sum + 1];

  memset(dp, INT_MAX - 1, sizeof(dp));

  for (int i = 0; i <= n; i++) {
    dp[i][0] = 0;
  }

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= sum; j++) {
      if (j >= coins[i - 1]) {
        dp[i][j] = std::min(dp[i - 1][j], dp[i][j - coins[i - 1]] + 1);
      } else {
        dp[i][j] = dp[i - 1][j];
      }
    }
  }
  return dp[n][sum];
}

int solve(const int coins[], const int n, const int sum) {
  int dp[n + 1][sum + 1];

  memset(dp, 0, sizeof(dp));

  for (int i = 0; i <= n; i++) {
    dp[i][0] = 1;
  }

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= sum; j++) {
      if (j >= coins[i - 1]) {
        dp[i][j] = dp[i - 1][j] + dp[i][j - coins[i - 1]];
      } else {
        dp[i][j] = dp[i - 1][j];
      }
    }
  }
  return dp[n][sum];
}

int main() {

  std::istream *in = &std::cin;

  if (TEST) {
    in = new std::ifstream("input.txt");
    if (in->fail()) {
      std::cout << "errore" << std::endl;
      return 2;
    }
  }

  std::istream &cin = *in;

  // CODICE
  int n, t, sum, coins[10000];
  cin >> t;
  while (t--) {
    cin >> n >> sum;
    for (int i = 0; i < n; i++) {
      cin >> coins[i];
    }
    std::cout << min(coins, n, sum) << std::endl;
  }

  if (TEST)
    delete in;
}
