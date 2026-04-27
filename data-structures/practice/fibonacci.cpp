#include <bits/stdc++.h>
using namespace std;

// calcolare il numero di fibonacci per n
// fibonacci è definito come c_i = c_(i-1) + c_(i-2)
int fibonacci(int n) {
    int dp[n+1];
    dp[0] = 0;
    dp[1] = 1;

    for(int i = 2; i <= n; i++) {
        dp[i] = dp[i-1] + dp[i-2];
    }

    return dp[n];
}

int main(int argc, char const *argv[]) {
    /* code */
    int n;
    cin >> n;
    cout << fibonacci(n) << endl; 
    return 0;
}
