#include <bits/stdc++.h>
using namespace std;

bool subset_sum(vector<int>& set, int sum) {

    int n = set.size();
    bool dp [n+1][sum+1];

    memset(dp, false, sizeof(dp));
    for(int i = 0; i <= n; ++i) {
        dp[i][0] = true;
    }

    for(int i = 1; i <= n; ++i) {
        for(int j = 1; j <= sum; ++j) {
            if( (j-set[i-1]) >= 0) {
                dp[i][j] = dp[i-1][j-set[i-1]] || dp[i-1][j];
            } else {
                dp[i][j] = dp[i-1][j];
            }
        }
    }

    for(int i = 0; i < n; ++i) {
        for(int j = 1; j <= sum; ++j) {
            cout << dp[i][j] << ' ';
        }
        cout << endl;
    }

    return dp[n-1][sum];
}

int main(int argc, char const *argv[]) {
    int n, sum;
    vector<int> set;
    cin >> n >> sum;
    set.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> set[i]; 
    }
    for (int i = 0; i < n; ++i) {
        cout << set[i] <<  ' '; 
    }
    cout << endl;
    cout<<subset_sum(set, sum) << endl;
    return 0;
}