
// Matrix Multiplication - Host only version

#include <iostream>

#define ROWS 5
#define COLS 10

void matrixMultiplication(const float *A, const float *B, float *C,
                          size_t Arows, size_t Acols, size_t Bcols) {
  for (size_t r = 0; r < Arows; r++) {
    for (size_t c = 0; c < Bcols; c++) {
      C[r * Bcols + c] = 0.0;
      for (size_t k = 0; k < Acols; k++) {
        C[r * Bcols + c] += A[r * Acols + k] * B[k * Bcols + c];
      }
    }
  }
}

int main() {

  size_t M1cols = 6;
  size_t M1rows = 4;
  size_t M2cols = 8;
  size_t M2rows = M1cols;

  float M1[M1rows * M1cols];
  float M2[M2rows * M2cols];
  float MO[M1rows * M2cols];

  float INITVALUE = 0.5;

  for (size_t r = 0; r < M1rows; r++) {
    for (size_t c = 0; c < M1cols; c++) {
      M1[r * M1cols + c] = 1.0;
    }
  }
  for (size_t r = 0; r < M2rows; r++) {
    for (size_t c = 0; c < M2cols; c++) {
      M2[r * M2cols + c] = INITVALUE;
    }
  }

  matrixMultiplication(M1, M2, MO, M1rows, M1cols, M2cols);

  for (size_t r = 0; r < M1rows; r++) {
    printf("\n");
    for (size_t c = 0; c < M2cols; c++) {
      printf(" %f", MO[r * M2cols + c]);
    }
  }

  return 0;
}
