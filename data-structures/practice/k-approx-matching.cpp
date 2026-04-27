#include <bits/stdc++.h>
using namespace std;


int min_k_approx(string & t, string & p) {
    int n = t.size();
    int m = p.size();

    int dp[n+1][m+1];

    for(int i = 0; i <= n; ++i) {
        dp[i][0] = i;
    }

    for(int i = 0; i <= m; ++i) {
        dp[0][i] = 0;
    }

    for(int i = 1; i <= n;  ++i) {
        for(int j = 1; j <= m; ++j) {
            int delta = 1;
            if (t[i] == p[j]) {
                delta = 0;
            }
            dp[i][j] = min(dp[i-1][j-1] + delta,
                min(
                    dp[i-1][j] + 1,
                    dp[i][j-1] + 1
                )
                );
        }
    }

    return *min_element(dp[n], dp[n] + m);
}

int main() {
    string t, p;

    cin >> t >> p;

    cout << min_k_approx(t, p) << endl;
}