#include <bits/stdc++.h>

#define TEST 1
int solve(const int profits[], const int weights[], const int n, const int c) {
  int dp[c + 1];

  memset(dp, 0, sizeof(dp));

  for (int i = 1; i <= c; i++) {
    for (int j = 0; j < n; j++) {
      if (weights[j] <= i) {
        dp[i] = std::max(dp[i], profits[j] + dp[i - weights[j]]);
      }
    }
  }
  return dp[c];
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
