#include <bits/stdc++.h>

#define TEST 1

int solve(const int n) {
  int dp[n + 1];
  dp[3] = 4;
  for (int i = 4; i <= n; ++i) {
    dp[i] = dp[i - 3] + dp[i - 2] + dp[i - 1];
  }
  return dp[n];
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
  int n, t;
  cin >> t;
  while (t--) {
    cin >> n;
    std::cout << solve(n) << std::endl;
  }

  if (TEST)
    delete in;
}
