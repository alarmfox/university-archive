#include <bits/stdc++.h>
using namespace std;

void print_sol(int sol[], int p[], int n) {
  int sum = 0;
  for (size_t i = 0; i < n; i++) {
    sum += p[sol[i]];
    cout << p[sol[i]] << ' ';
  }
  cout << "sol=" << sum << endl; 
}

vector<int> knapsack(const int w[], const int p[], int n, int c) {
  int dp[n+1][c+1];
  int res = 0;
  vector<int> sol;
  // capienza = 0
  for (size_t i = 0; i <= n; ++i) {
    dp[i][0] = 0;
  }
  // tifosi = 0
  for (size_t i = 0; i <= c; ++i) {
    dp[0][i] = 0;
  }

  for(size_t i = 1; i <= n; ++i){
    for (size_t j = 1; j<= c; ++j) {
      // se ho capienza metto l'oggetto nello zaino
      if (w[i-1] <= j) {
        dp[i][j] = max(dp[i-1][j], dp[i-1][j-w[i-1]] + p[i-1]);
      } else {
        // altrimenti il profitto è quello dell'elemento precedente
        dp[i][j] = dp[i-1][j];
      }
    }
  }


  res = dp[n][c];
  for (size_t i = n, j = c; i > 0 && res > 0; i--) {
    
    if (res == dp[i-1][j]) {
     continue;   
    } 
    // l'elemento è incluso
    sol.push_back(i-1);
              
    // si decrementa res e j per saltare ad un altra colonna
    res -= p[i-1];
    j -= w[i-1];
  }

  reverse(sol.begin(), sol.end());
  return sol;
}

int main (int argc, char *argv[]) {

  // gruppi di persone, capacità
  vector<pair<vector<int>, int>> test_cases = {
    {{5, 7, 20, 2, 5}, 16},
    {{15, 27, 40, 22, 53}, 100},
    {{15, 2, 20, 24, 50}, 46},
  };

  for(const auto& test_case: test_cases) {
    vector<int> w = test_case.first;
    int c = test_case.second;
    vector<int> sol = knapsack(w.data(), w.data(), w.size(), c);
    print_sol(sol.data(), w.data(), sol.size());
  }
  
  return 0;
}
