int pivotIndex(int *nums, int numsSize) {
  int pivot = 0, sum1 = 0, sum2 = 0;

  while (pivot < numsSize) {
    sum1 = 0;
    sum2 = 0;

    for (int i = 0; i < pivot; ++i) {
      sum1 += nums[i];
    }

    for (int i = pivot + 1; i < numsSize; ++i) {
      sum2 = nums[i];
    }

    if (sum1 == sum2)
      return pivot;
    pivot++;
  }

  return -1;
}

int main(void) {
  int a[] = {1, 7, 3, 6, 5, 6};
  pivotIndex(a, 6);
  return 0;
}
