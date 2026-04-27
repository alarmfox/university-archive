#include <bits/stdc++.h>
using namespace std;

bool sort_cities(pair<int, int> a, pair<int, int> b) {
    if(a.second == b.second) {
        return a.first < b.first;
    }

    return a.second < b.second;
}

int max_bridges(vector<pair<int, int>>& cities) {
    sort(cities.begin(), cities.end(), sort_cities);
    int n = cities.size();
    int dp[n];
    for (int i=0; i<n; i++)
        dp[i] = 1;

    for(int i = 1; i < n; ++i) {
        for(int j = 0; j < i; ++j) {
            if(cities[i].first >= cities[j].first) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
    }

    return *max_element(dp, dp+n);
}

int main() {
    int n, N;
    vector<pair<int, int>> cities;
    cin >> N;
    while (N--) {

        cin >> n;
        cities.resize(n);

        for(int i = 0; i < n; ++i) {
            cin >> cities[i].first;
        }
    
        for(int i = 0; i < n; ++i) {
            cin >> cities[i].second;
        }
        
        cout << max_bridges(cities) << endl;
    }
}