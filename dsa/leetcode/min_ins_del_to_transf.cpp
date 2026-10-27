#include <bits/stdc++.h>

#define TEST 1
// Per trasformare a in b trovo la lunghezza della LCS e posso dire che
// il numero di inserimento è b.len() - len(LCS) (caratteri in meno)
// il numero di cancellazioni è a.len() - len(LCS) (caratteri in più)
int solve(const std::string &a, const std::string &b) {
  int dp[a.size() + 1][b.size() + 1];

  for (int i = 0; i <= a.size(); i++) {
    for (int j = 0; j <= b.size(); j++) {
      if (i == 0 || j == 0) {
        dp[i][j] = 0;
      } else if (a[i-1] == b[j-1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }
  return dp[a.size()][b.size()];
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
    int lcs = solve("heap", "pea");
    std::cout << "lcs: " << lcs << std::endl;
    std::cout << "canc: " << std::string("heap").size() - lcs << std::endl;
    std::cout << "ins: " << std::string("pea").size() - lcs << std::endl;
  }

  if (TEST)
    delete in;
}
