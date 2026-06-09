#include <bits/stdc++.h>

#define TEST 0

std::string solve(const std::string &a, const std::string &b) {
  int dp[a.size() + 1][b.size() + 1];

  for (size_t i = 0; i <= a.size(); i++) {
    for (size_t j = 0; j <= b.size(); j++) {
      if (i == 0 || j == 0)
        dp[i][j] = 0;
      else if (a[i - 1] == b[j - 1])
        dp[i][j] = dp[i - 1][j - 1] + 1;
      else
        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
    }
  }
  std::string lcs;
  int i = a.size(), j = b.size();
  while (i > 0 && j > 0) {
    if (a[i - 1] == b[j - 1]) {
      lcs.push_back(a[i - 1]);
      i--;
      j--;
    } else if (dp[i - 1][j] > dp[i][j - 1])
      i--;
    else
      j--;
  }
  std::reverse(lcs.begin(), lcs.end());
  return lcs;
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
    cin >> a;
    cin >> b;
    std::cout << solve(a, b) << std::endl;
  }

  if (TEST)
    delete in;
}
