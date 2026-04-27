#include <stdio.h>

#define N 4
#define M 4

void print_matrix(const unsigned char m[N][M]) {
  for (int i = 0; i < N; ++i) {
  int count2 = 0;
    for (int j = 0; j < M; ++j) {
      printf("%d ", m[i][j]);
    }
    printf("\n");
  }
}

int count_in_byte(const unsigned char c, const unsigned char mask[],
                  const int n) {
  int count = 0;
  for (int i = 0; i < n; i++) {
    if (c & mask[i])
      count++;
  }
  return count;
}

void find_min(unsigned char m[N][M], int *min, int *minR) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      if (m[i][j] < *min) {
        *min = m[i][j];
        *minR = i;
      }
    }
  }
}

void find_max(unsigned char m[N][M], int *max, int *maxR) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      if (m[i][j] > *max) {
        *max = m[i][j];
        *maxR = i;
      }
    }
  }
}

void swap_rows(unsigned char m[N][M], int r1, int r2) {
  unsigned char tmp;
  for (int i = 0; i < M; i++) {
    tmp = m[r1][i];
    m[r1][i] = m[r2][i];
    m[r2][i] = tmp;
  }
}

int main() {
  unsigned char m[N][M] = {
      {0, 128, 0, 2},
      {63, 28, 2, 3},
      {31, 32, 255, 4},
      {15, 18, 3, 5},
  };
  const unsigned char mask[8] = {0x01, 0x02, 0x04, 0x08,
                                 0x10, 0x20, 0x40, 0x80};
  int min = 255, max = -1, minR = 0, maxR = 0, count = 0;

  find_min(m, &min, &minR);
  find_max(m, &max, &maxR);
  print_matrix(m);

  swap_rows(m, minR, maxR);

  print_matrix(m);

  if (N == M) {
    for (int i = 0; i < N; i++) {
      count += count_in_byte(m[i][i], mask, 8);
    }
    printf("\nsquared;count=%d\n", count);
  }

  printf("\nmin=%d\nmax=%d\nmin-row=%d\nmax-row=%d\n", min, max, minR, maxR);
}
