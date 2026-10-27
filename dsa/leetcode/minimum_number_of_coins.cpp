#include <bits/stdc++.h>

#define TEST 1

int solve(const int n) {

  int dp[n];
  dp[0] = 0;
  for (int i = 1; i <= n; i++) {
    if (i % 2 == 0) {
      dp[i] = dp[i / 2] + 1;
    } else {
      dp[i] = dp[i - 1] + 1;
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
    std::cout << solve(n) << std::endl;
  }

  if (TEST)
    delete in;
}
