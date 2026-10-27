#include <bits/stdc++.h>
using namespace std;

int min_square(const int n) {
  
  // inizializzo il vettore dp
	int dp[n+1];

  dp[0] = 0;
  dp[1] = 1;
  for(int i = 2; i <= n; ++i) {
    dp[i] = INT_MAX;
    for(int j = 1; (j*j) <= i; ++j) {
      // applico la formula ricorsiva
      dp[i] = min(dp[i], 1 + dp[i - (j*j)]);
    }
  }

  return dp[n];
}

int main (int argc, char *argv[]) {
  
  vector<int> test_cases = {
    100,
    25,
    625,
    63,
  };

  int res;
  for (size_t i = 0; i < test_cases.size(); ++i ) {
    cout << "TEST: " << test_cases[i] << ": ";
    res = min_square(test_cases[i]);
    cout << "ANS: " << res << endl;
  }
  return 0;
}
