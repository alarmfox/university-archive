#include <bits/stdc++.h>
using namespace std;

int cutting_rod(vector<int> & p) {
    int n = p.size();
    int dp[n+1];
    memset(dp, INT_MIN, sizeof(dp));
    dp[0] = 0;

    for (int i = 1; i <= n; ++i) {
        int max_val= INT_MIN;
        for(int j = 0; j < i; ++j) {
            max_val  = max(max_val, p[j] + dp[i-j-1]);
        }
        dp[i] = max_val;
    }

    return dp[n];

}


int main(int argc, char const *argv[]) {
    int n;
    vector<int> set;
    cin >> n;
    set.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> set[i]; 
    }
    cout<<cutting_rod(set) << endl;
    return 0;
}