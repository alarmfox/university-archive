#include <bits/stdc++.h>

#define TEST 1
#define DEBUG 1

// Due ladri mettono a segno un colpo alla banca d’Italia. Recuperano un bottino composto da monete
// d’oro di diverso valore. Devono dividerselo il più equamente possibile, ossia devono minimizzare la
// differenza tra il valore che ciascuno di loro ottiene. Scrivere un algoritmo per determinare la
// suddivisione, stampando in output la differenza (positiva) tra il valore che i due ladri ottengono
// dividendosi il bottino.
// Il bottino contiene al massimo 50 monete, il valore di ciascuna moneta varia da 1 a 1000. Il valore
// di una moneta può occorrere più di una volta.

int solve(const int nums[], const int n) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += nums[i];
  }

  bool dp[n + 1][sum + 1];

  for (int i = 0; i <= n; ++i) {
    dp[i][0] = true;
  }

  for (int i = 1; i <= sum; ++i) {
    dp[0][i] = false;
  }

  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= sum; j++) {
      if (j >= nums[i - 1]) {
        dp[i][j] = dp[i - 1][j] || dp[i - 1][j - nums[i - 1]];
      } else {
        dp[i][j] = dp[i - 1][j];
      }
    }
  }

  if (DEBUG) {
    for (int i = 0; i <= n; ++i) {
      for (int j = 0; j <= sum; ++j) {
        std::cout << dp[i][j] << ' ';
      }
      std::cout << std::endl;
    }
  }

  int diff = INT_MAX;
  for (int j = sum / 2; j >= 0; j--) {
    if (dp[n][j]) {
      diff = sum - 2 * j;
      return diff;
    }
  }
  return -1;
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
