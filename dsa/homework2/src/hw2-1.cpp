#include <bits/stdc++.h>
using namespace std;

#define N 100
#define M 100


void print_matrix(int mat[N][M], int n, int m) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      cout << mat[i][j] << ' ';
    }
    cout << endl;
  }
}
bool is_in(int v, int start, int end) {
  return v >= start && v < end;
}
bool is_valid(int mat[N][M], int n, int m, int i, int j, int pvalue, bool visited[N][M]) {
  // controllo dei confini
  if (!is_in(i, 0, n) || !is_in(j, 0, m)) {
    return false;
  } 

  // controllo se la mossa è stata già eseguita
  if(visited[i][j]) {
    return false;
  }

  return pvalue < mat[i][j];
  
}

void _find_increasing_path(int mat[N][M], int n, int m, int i, int j, bool visited[N][M], int& current, int &res) {
  res = max(res, current);

  visited[i][j] = true;
  current ++;

  // sotto
  if (is_valid(mat, n, m, i+1, j, mat[i][j], visited)) {
    _find_increasing_path(mat,n, m, i+1, j, visited, current, res);
  }

  // destra
  if (is_valid(mat, n, m, i, j+1, mat[i][j], visited)) {
    _find_increasing_path(mat,n, m, i, j+1, visited, current, res);
  }

  // sopra
  if (is_valid(mat, n, m, i-1, j, mat[i][j], visited)) {
    _find_increasing_path(mat,n, m, i-1, j, visited, current, res);
  }

  // sinistra
  if (is_valid(mat, n, m, i, j-1, mat[i][j], visited)) {
    _find_increasing_path(mat,n, m, i, j-1, visited, current, res);
  }

  // alto sinistra
  if (is_valid(mat, n, m, i-1, j-1, mat[i][j], visited)) {
    _find_increasing_path(mat,n, m, i-1, j-1, visited, current, res);
  }

  // alto destra
  if (is_valid(mat, n, m, i-1, j+1, mat[i][j], visited)) {
    _find_increasing_path(mat,n, m, i-1, j+1, visited, current, res);
  }
  
  // basso sinistra
  if (is_valid(mat, n, m, i+1, j-1, mat[i][j], visited)) {
    _find_increasing_path(mat,n, m, i+1, j-1, visited, current, res);
  }
  
  // basso destra
  if (is_valid(mat, n, m, i+1, j+1, mat[i][j], visited)) {
    _find_increasing_path(mat,n, m, i+1, j+1, visited, current, res);
  }
  
  visited[i][j] = false;
  current --;
}

int find_increasing_path(int mat[N][M], int n, int m) {
  bool visited[N][M];
  for (int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      visited[i][j] = false;
    }
  }
  int res = 0;
  int current = 0;

  _find_increasing_path(mat, n, m, 0, 0, visited, current, res);

  return res;
}

int main (int argc, char *argv[]) {
  
  int test_cases [3][N][M] = {
    {
      {1,2,4},
      {6,5,0},
      {7,8,3},
    },

    {
      {9,6,5},
      {4,0,20},
      {2,3,1},
    },
  
    {
      {5,2,4},
      {6,9,1},
      {0,8,3},
    },
  };

  for (size_t i = 0; i < 3; ++i) {

    print_matrix(test_cases[i], 3, 3);
    int res = find_increasing_path(test_cases[i], 3,3);
    cout << "RES: " << res << endl;

  }
  return 0;
}
