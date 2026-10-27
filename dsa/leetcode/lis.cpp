#include <bits/stdc++.h>

#define TEST 1

int solve(const int n, const int nums[]) {
  int dp[n + 1];

  dp[0] = 0;
  dp[1] = 1;
  for (int i = 1; i <= n; ++i) {
    int max = -1;
    for (int j = 1; j < i; ++j) {
      if (nums[i-1] >= nums[j-1] && dp[j] > max) {
        max = dp[j];
      }
    }
    if (max == -1) {
      dp[i] = 1;
    } else {
      dp[i] = max + 1;
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
  int n, t, nums[10000];
  cin >> t;
  while (t--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> nums[i];
    }
    std::cout << solve(n, nums) << std::endl;
  }

  if (TEST)
    delete in;
}
