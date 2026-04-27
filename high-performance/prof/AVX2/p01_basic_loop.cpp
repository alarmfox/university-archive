#include <iostream>
#include <chrono>

void foo(double *__restrict__ p1, double *__restrict__ p2, size_t start,
         size_t end) {

  double *pA1 = static_cast<double *>(__builtin_assume_aligned(p1, 16));
  double *pA2 = static_cast<double *>(__builtin_assume_aligned(p2, 16));

  for (size_t i = start; i < end; ++i) {
    pA1[i] = pA1[i] - pA2[i];
    pA1[i] += 1;
  }
}

#define N 10000000
double v[N];
double u[N];

int main() {

  for (size_t i = 0; i < N; ++i) {
    v[i] = i;
    u[i] = i - 1;
  }

  using namespace std::chrono;

  high_resolution_clock::time_point t1 = high_resolution_clock::now();

  foo(v, u, 0, N);

  high_resolution_clock::time_point t2 = high_resolution_clock::now();

  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

  std::cout << "It took me " << time_span.count() << " seconds.";
  std::cout << std::endl;

  return 0;
}

/*

#include <iostream>


void multiply_and_add(const float* a, const float* b, const float* c, float* d)
{

        for(int i=0; i<8; i++) {
                d[i] = a[i] * b[i];
                d[i] = d[i] + c[i];
        }
}



int main(){

        float a[8] = { 2.4, -4.543, 3243.4023, +32434.543, -434324.235, 435.54,
1230.000023, 3423.34}; float b[8] = { 232.4, +4.54323, 3243.4023, +32434.543,
-434324.235, 435.54, 0.000023, 3.34}; float c[8] = { -322.4, -3.543, 323.4023,
-324.543, -1324.235, 435.54, 0.02, 3423.34}; float d[8] ; // = { 32.0024,
-4.52343, 3243.4023, +324.543, +3412324.235, 435.54, 0.000023, 3423.34};

        multiply_and_add(a,b,c,d);

        return 0;
}

*/
