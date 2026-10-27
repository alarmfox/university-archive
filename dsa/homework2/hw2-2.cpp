#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

void print_array(const int a[], const int n) {
  for (size_t i = 0; i < n; ++i) {
    cout << a[i] << ' ';
  }
}

int max_sum_increasing_subsequence(const int arr[], const int n) {

  int res = INT_MIN;
	int dp[n];

  // inizializzazione vettore dp
	for (size_t i = 0; i < n; ++i) {
		dp[i] = arr[i];
  }

  // calcolo della somma massima per la sottosequenza che termina in i-esima posizione
	for (size_t i = 1; i < n; ++i) {
    for (size_t j = 0; j < i; ++j) {
      // sono in una sottosequenza crescente?
      if (arr[i] > arr[j]) {
        // incremento la somma
        dp[i] = max(dp[i], dp[j] + arr[i]);
      }
    }
  }

  // calcolo del valore massimo
	for (size_t i = 0; i < n; ++i) {
		if (res < dp[i]) {
			res = dp[i];
    }
  }

	return res;
}

int main (int argc, char *argv[]) {
  
  
  vector<vector<int>> test_cases = {
    {
      10,5,14,3,
    },
    {
      3,4,5,10
    },
    {
      10,5,4,3,
    }
  };

  int res;
  for (size_t i = 0; i < test_cases.size(); ++i ) {
    cout << "TEST " << i + 1 << ": ";
    print_array(test_cases[i].data(), test_cases[i].size());
    res = max_sum_increasing_subsequence(test_cases[i].data(), test_cases[i].size());
    cout << "\nLIS " << res << endl;
  }

  return 0;
}
