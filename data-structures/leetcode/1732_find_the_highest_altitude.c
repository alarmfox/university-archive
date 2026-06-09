int largestAltitude(int *gain, int gainSize) {
  int current = 0, max = 0;
  for (int i = 0; i < gainSize; ++i) {
    current += gain[i];
    if (current > max)
      max = current;
  }

  return max;
}

int main(void) {
  int a[] = {-4, -3, -2, -1, 4, 3, 2};
  largestAltitude(a, 7);
  return 0;
}
