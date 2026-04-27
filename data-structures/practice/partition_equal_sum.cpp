#include <bits/stdc++.h>

#define TEST 1

int solve(const int nums[], const int n) {
  int sum = 0;
  for (int i = 0; i < n; ++i) {
    sum += nums[i];
  }

  if (sum % 2 != 0)
    return false;

  // devo vedere se esiste una sottosequenz con somma = sum/2
  // prima colonna a zero
  bool dp[n + 1][sum / 2 + 1];
  for (int i = 0; i <= n; ++i) {
    dp[i][0] = true;
  }

  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= sum / 2; j++) {
      if (j >= nums[i]) {
        dp[i][j] = dp[i - 1][j] || dp[i - 1][j - nums[i]];
      } else {
        dp[i][j] = dp[i - 1][j];
      }
    }
  }

  return dp[n][sum / 2];
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
  int n, t, nums[100000];
  cin >> t;
  while (t--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> nums[i];
    }
    std::cout << solve(nums, n) << std::endl;
  }

  if (TEST)
    delete in;
}
