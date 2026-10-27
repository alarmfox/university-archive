#include <bits/stdc++.h>

#define TEST 1

int solve(const int nums[], const int n) {
  
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
    for (int i = 0; i <n ;i++) {
      cin >> nums[i];
    }
    std::cout << solve(nums, n) << std::endl;
  }

  if (TEST)
    delete in;
}
