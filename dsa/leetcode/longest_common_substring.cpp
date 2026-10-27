#include <bits/stdc++.h>

#define TEST 1

int solve(const std::string& a, const std::string& b) {
  int dp[a.size() + 1][b.size() + 1];
  int result = INT_MIN;

  for (int i = 0; i <= a.size(); ++i) {

    for (int j = 0; j <= b.size(); j++) {
      if (i == 0 || j == 0) {
        dp[i][j] = 0;
      } else if (a[i-1] == b[j-1]) {
        dp[i][j] = 1 + dp[i-1][j-1];
        result = std::max(result, dp[i][j]);
      } else {
          dp[i][j] = 0;
      }
    }
  }
  return result;
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
  int t;
  std::string a, b;
  cin >> t;
  while (t--) {
    cin >> a >> b;
    std::cout << solve(a, b) << std::endl;
  }

  if (TEST)
    delete in;
}
