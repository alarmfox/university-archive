#include <bits/stdc++.h>

#define TEST 1

// Given an input string and a dictionary of words,
// find out if the input string can be segmented into a space-separated sequence
// of dictionary words.
// { i, like, sam, sung, samsung, mobile, ice,
//  cream, icecream, man, go, mango}
// Input:  ilike
// Output: Yes
// The string can be segmented as "i like".

bool solve(const std::vector<std::string> &d, const std::string &w) {

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
  int n, t;
  std::vector<std::string> d;
  std::string a;
  cin >> t;
  while (t--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a;
      d.push_back(a);
    }
    cin >> a;
    std::cout << solve(d, a) << std::endl;
  }

  if (TEST)
    delete in;
}
