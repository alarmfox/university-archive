#include <bits/stdc++.h>

#define TEST 1

int solve(const std::string &a, const std::string &b) {
  int dp[a.size() + 1][b.size() + 1];
  std::string lcs, res;

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
  // // string to store the shortest supersequence
  // std::string str;
  //
  // // Start from the bottom right corner and one by one
  // // push characters in output string
  // int i = a.size(), j = b.size();
  // while (i > 0 && j > 0) {
  //   // If current character in X and Y are same, then
  //   // current character is part of shortest supersequence
  //   if (a[i - 1] == b[j - 1]) {
  //     // Put current character in result
  //     str.push_back(a[i - 1]);
  //
  //     // reduce values of i, j and index
  //     i--, j--;
  //   }
  //
  //   // If current character in X and Y are different
  //   else if (dp[i - 1][j] > dp[i][j - 1]) {
  //     // Put current character of Y in result
  //     str.push_back(b[j - 1]);
  //
  //     // reduce values of j and index
  //     j--;
  //   } else {
  //     // Put current character of X in result
  //     str.push_back(a[i - 1]);
  //
  //     // reduce values of i and index
  //     i--;
  //   }
  // }
  //
  // // If Y reaches its end, put remaining characters
  // // of X in the result string
  // while (i > 0) {
  //   str.push_back(a[i - 1]);
  //   i--;
  // }
  //
  // // If X reaches its end, put remaining characters
  // // of Y in the result string
  // while (j > 0) {
  //   str.push_back(b[j - 1]);
  //   j--;
  // }

  // reverse the string and return it
  // reverse(str.begin(), str.end());
  // return str;
  return a.size() + b.size() - dp[a.size()][b.size()];
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
