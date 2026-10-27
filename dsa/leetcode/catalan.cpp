#include <bits/stdc++.h>
using namespace std;

// calcolare il numero di catal per n
// definito come C_i+1 = \sum_j=0^i C_j*C_(i-j)
int catalan(int n) {
    int dp[n+1];
    dp[0] = 1;

    for(int i = 1; i <= n; ++i) {
        int sum = 0;
        for(int j = 1; j <= i; ++j) {
            sum += dp[j-1]*dp[i-j];
        }
        dp[i] = sum;
    }

    return dp[n];
}

int main(int argc, char const *argv[]) {
    /* code */
    int n;
    cin >> n;
    cout << catalan(n) << endl; 
    return 0;
}