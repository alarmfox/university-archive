#include <bits/stdc++.h>

#define TEST 1

int solve(const int nums[], const int n, const int r) {
  int count = 0;
  for (int i = 0; i < n; i++) {
    if (nums[i] < r) {
      count++;
    } else if (nums[i] == r) {
      return count;
    } else if (nums[i] > r) {
      return -1;
    }
  }
  return count;
}

int main() {

  std::istream *in = &std::cin;

  if (TEST) {
    in = new std::ifstream("input.txt");
    if (in->fail()) {
      std::cout << "errore" << std::endl;
      return 2;
    }
  }

  std::istream &cin = *in;

  // CODICE
  int n, t, nums[10000];
  cin >> t;
  while (t--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> nums[i];
    }
    cin >> q;
    std::cout << solve(n, nums, q) << std::endl;
  }

  if (TEST)
    delete in;
}
