#include <bits/stdc++.h>
using namespace std;

bool partition_set_eq_sum(vector<int>& v) {
    int sum = accumulate(v.begin(), v.end(), 0);
    size_t n = v.size();

    bool dp[n+1][(sum/2) + 1];
    memset(dp, false, sizeof(dp));

    for(size_t i = 0; i < n; ++i) {
        dp[i][0] = true;
    }

    for(int i = 1; i < sum/2; ++i) {
        dp[0][i] = false;
    }

    for(size_t i = 1; i <= n; ++i) {
        for(int j = 1; j <= sum/2; j++) {
            dp[i][j] = dp[i-1][j];

            if(j >= v[i-1]) {
                dp[i][j] |= dp[i-1][j-v[i-1]];
            }
        }
    }
    return dp[n][sum/2];
}

int main() {
    int n;
    vector<int> v;
    cin>>n;
    v.resize(n);

    for(size_t i = 0; i < v.size(); ++i) {
        cin >> v[i];
    }

    cout << partition_set_eq_sum(v) << endl;
}