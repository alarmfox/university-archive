#include <stdio.h>
#include <stdlib.h>

// You are given two integer arrays poly1 and poly2, where the element at index
// i in each array represents the coefficient of xi in a polynomial.
//
// Let A(x) and B(x) be the polynomials represented by poly1 and poly2,
// respectively.
//
// Return an integer array result of length (poly1.length + poly2.length - 1)
// representing the coefficients of the product polynomial R(x) = A(x) * B(x),
// where result[i] denotes the coefficient of xi in R(x).

int *multiply_polynomials(int *a, int asize, int *b, int bsize, int *rsize) {

  *rsize = asize + bsize - 1;
  int *r = malloc(sizeof(int) * *rsize);

  for (int i = 0; i < asize; ++i) {
    for (int j = 0; j < bsize; ++j) {
      r[i + j] += a[i] * b[j];
    }
  }

  return r;
}

int main(void) {
  int a[] = {3, 2, 5};
  int b[] = {1, 4};
  int *r = NULL;
  int rsize = -1;

  int asize = sizeof(a) / sizeof(a[0]);
  int bsize = sizeof(b) / sizeof(b[0]);

  r = multiply_polynomials(a, asize, b, bsize, &rsize);

  for (int i = 0; i < rsize; ++i) {
    printf("%d ", r[i]);
  }

  free(r);
}
