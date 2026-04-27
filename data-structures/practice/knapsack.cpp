#include <bits/stdc++.h>

#define TEST 1
int solve(const int profits[], const int weights[], const int n, const int c) {
  int dp[n + 1][c + 1];

  for (int i = 0; i <= n; ++i) {
    dp[i][0] = 0;
  }

  for (int i = 0; i <= c; ++i) {
    dp[0][i] = 0;
  }

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= c; j++) {
      if ((j - weights[i]) >= 0) {
        dp[i][j] =
            std::max(dp[i - 1][j], dp[i - 1][j - weights[i]] + profits[i]);
      } else {
        dp[i][j] = dp[i - 1][j];
      }
    }
  }
  return dp[n][c];
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
  int n, t, c, profits[10000], weights[10000];
  cin >> t;
  while (t--) {
    cin >> n >> c;
    for (int i = 0; i < n; i++) {
      cin >> profits[i];
    }
    for (int i = 0; i < n; i++) {
      cin >> weights[i];
    }
    std::cout << solve(profits, weights, n, c) << std::endl;
  }

  if (TEST)
    delete in;
}
