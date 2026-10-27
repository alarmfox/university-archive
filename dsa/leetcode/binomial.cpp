#include <bits/stdc++.h>
using namespace std;


// calcolare il coefficiente binomiale (n,k)
// formula ricorsiva:
// C(n,k) = C(n-1, k-1) + C(n-1, k)
// C(n,0) = C(n, n) = 1
int binomial(int n, int k) {

    int dp[n+1][k+1];

    for(int i = 0; i <= n; ++i) {
        for (int j = 0; j <= min(i, k); ++j) {
            if(j == 0 || i == j) {
                dp[i][j] = 1;
            } else {
                dp[i][j] = dp[i-1][j-1] + dp[i-1][j];
            }
        }
    }

    return dp[n][k];
}

int main(int argc, char const *argv[]) {
    /* code */
    int n,k;
    cin >> n >> k;
    cout << binomial(n, k) << endl; 
    return 0;
}