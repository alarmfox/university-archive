
#define REP 100000
#define SIZE 2 * 4096

double vinput1[SIZE] __attribute__((aligned(32)));
double vinput2[SIZE] __attribute__((aligned(32)));
double voutput[SIZE] __attribute__((aligned(32)));

inline void complexArrayMult(double *__restrict__ p1, double *__restrict__ p2,
                             double *__restrict__ p3, int size) {

  double *pA1 = static_cast<double *>(__builtin_assume_aligned(p1, 32));
  double *pA2 = static_cast<double *>(__builtin_assume_aligned(p2, 32));
  double *pA3 = static_cast<double *>(__builtin_assume_aligned(p3, 32));

  for (int i = 0; i < size; i += 2) {
    pA3[0] = pA1[0] * pA2[0] - pA1[1] * pA2[1];
    pA3[1] = pA1[0] * pA2[1] + pA1[1] * pA2[0];
    pA1 += 2;
    pA2 += 2;
    pA3 += 2;
  }
}

void kernel() {

  for (int j = 0; j < REP; j++) {
    complexArrayMult(vinput1, vinput2, voutput, SIZE);
  }
}
