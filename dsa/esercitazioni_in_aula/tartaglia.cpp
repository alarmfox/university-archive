#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

const vector<int> solution(const int k);

#define DEBUG 0

int main() {
    const int n1 = 5;
    const int ris1 [n1]= {1, 4, 6, 4, 1};

    const auto res1 = solution(n1);

    for (int i = 0; i < n1; ++i) {
        assert(ris1[i] == res1[i]);
    }

    const int n2 = 3;
    const int ris2 [n2]= {1, 2, 1};

    const auto res2 = solution(n2);

    for (int i = 0; i < n2; ++i) {
        assert(ris2[i] == res2[i]);
    }
}

const vector<int> solution(const int n) {
    vector<int> res;

    int tartaglia[n][n];

    tartaglia[0][0] = 1;

    for (int i = 1; i < n; ++i) {
        tartaglia[0][i] = 0;
    }

    for (int i = 1; i < n; ++i) {
        tartaglia[i][0] = 1;
        for (int j = 1; j < n; ++j) {
            tartaglia[i][j] = tartaglia[i - 1][j - 1] + tartaglia[i - 1][j];
        }
    }

    if (DEBUG) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << tartaglia[i][j] << ' ';
            }
            cout << endl;
        }
    }

    for (int i = 0; i < n; ++i) {
        res.push_back(tartaglia[n - 1][i]);
    }
    return res;
}

