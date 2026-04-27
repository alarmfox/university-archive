#include <bits/stdc++.h>

#define TEST 1


int solve(const int n) {

  int dp[n + 1];
  dp[0] = 0;
  dp[1] = 1;
  dp[2] = 1;

  for (int i = 3; i <= n; ++i) {
    dp[i] = 0;
    for (int j = 1; j < i; ++j) {
      dp[i] = std::max(dp[i], std::max(j * (i - j), j * dp[i - j]));
    }
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
    std::cout << solve(n)<< std::endl;
  }

  if (TEST)
    delete in;
}
