#include <bits/stdc++.h>
using namespace std;

int coin_change(vector<int> coins, int sum) {
    int n  = coins.size();
    int dp[n + 1] [sum+1];


    memset(dp, INT_MIN, sizeof(dp));
    dp[0][0] = 1;
    for(int i = 1; i <= n; ++i) {
        for(int j = 0; j <= sum; ++j) {
            // prendo la moneta
            if(coins[i-1] <= j) {
                dp[i][j] = min(dp[i][j], dp[i][j-coins[i-1]]);
            }
        }
    }

    return dp[n][sum];
}

int main(int argc, char const *argv[]) {
    int n, sum;
    vector<int> coins;
    cin >> n >> sum;
    coins.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> coins[i]; 
    }
    sort(coins.begin(), coins.end());
    cout<<coin_change(coins, sum) << endl;
    return 0;
}