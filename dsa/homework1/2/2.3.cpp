#include <iostream>
#include <vector>
#include <cassert>

typedef std::vector<std::vector<int>> Matrix;

#define DEBUG 1

std::ostream& print_matrix(const Matrix &m, std::ostream& out) {
  for (int i = 0; i<m.size(); ++i) {
    for (int j = 0; j < m[i].size(); ++j) {
      out<<m[i][j]<<' ';
    }
    out<<std::endl;
  }
  return out;
}

const int solution(const Matrix& M) {
  int area = 0, m = M.size(), n = M[0].size();
  Matrix dp(m, std::vector<int>(n));
  for (int i = m - 1; i >= 0; i--) {
    for (int j = n - 1; j >= 0; j--) {
      if (M[i][j] == 0) {
        // controlliamo che j+1 sia nel range della matrice, se si aggiungiamo 1 alla cella altrimenti assegnamo 1 come valore di partenza.
        dp[i][j] = j+1 >=n ? 1 : dp[i][j + 1] + 1;
      }
      else {
        dp[i][j] = 0;
      }
    }
  }
  

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      for (int row = i, columns = n; row < m && M[row][j] == 0; ++row) {
        columns = std::min(columns, dp[row][j]);
        area = std::max(area, (row - i + 1) * columns);
      }
    }

  }

  return area;
}

int main() {
  const std::vector<std::pair<Matrix, int>> test_cases = {
    {
    {
      {0,1,0,1,1},
      {0,1,0,0,0},
      {0,0,0,0,0},
      {0,1,1,0,1},
    }, 6
    },
    {
    {
      {1,1,0,1,1},
      {0,1,1,0,0},
      {1,0,0,0,0},
      {0,1,1,0,1},
    }, 4
    },
    {
    {
      {1,1,0,1,1},
      {0,0,0,0,0},
      {1,0,0,1,0},
      {0,0,0,0,1},
    }, 6
    }
  };
  int res;
  for (const auto tc : test_cases ) {
    res = solution(tc.first);
    assert(res == tc.second);
    if(DEBUG) {
      std::cout<<"input: \n";print_matrix(tc.first, std::cout)<<"risultato: "<<res<<"; risultato atteso: "<<tc.second<<"\n\n";
    }
  }
}