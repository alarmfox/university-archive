#include <iostream>

using namespace std;

long long int STEPS = 1 << 24;

double sum = 0;

void pi(int start, int count, double dx) {

  for (long long int i = start; i < (start + count); i++) {
    double x = (i + 0.5) * dx;
    sum = sum + 4.0 / (1 + x * x);
  }
}

// sequential version:

int main() {

  double dx = 1.0 / STEPS;

  pi(0, STEPS / 2, dx);

  pi(STEPS / 2, STEPS / 2, dx);

  double pi = sum * dx;

  std::cout << "\n pi: " << pi;

  return 0;
}
