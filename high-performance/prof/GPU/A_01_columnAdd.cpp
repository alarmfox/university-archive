
// Add the column elements of a matrix to an output vector - Host only version

#include <iostream>

#define ROWS 5
#define COLS 10

void kernel(float mat[ROWS][COLS], float vec[COLS], int rows, int cols) {
  for (int c = 0; c < cols; c++) {
    vec[c] = 0;
    for (int r = 0; r < rows; r++) {
      vec[c] += mat[r][c];
    }
  }
}

int main() {

  // float myMat[ROWS][COLS] = { {1,2,3,4,5,6,7,8,9,0}, {1,2,3,4,5,6,7,8,9,0},
  // {1,2,3,4,5,6,7,8,9,0}, {1,2,3,4,5,6,7,8,9,0}, {1,2,3,4,5,6,7,8,9,0} };
  float myMat[ROWS][COLS];
  float myVec[COLS];

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      myMat[r][c] = rand() / (float)RAND_MAX;
    }
  }

  kernel(myMat, myVec, ROWS, COLS);

  for (int r = 0; r < ROWS; r++) {
    std::cout << "\n";
    for (int c = 0; c < COLS; c++) {
      std::cout << myMat[r][c] << "  ";
    }
  }
  std::cout << "\n\n";
  for (int c = 0; c < COLS; c++) {
    std::cout << myVec[c] << "  ";
  }

  return 0;
}
