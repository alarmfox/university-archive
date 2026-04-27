#include<iostream>
#include <vector>
#include <cassert>

#define DEBUG 1


const int solution(const int n) {
    int pairs[n+1];
    pairs[0] = pairs[1] = 1;
 
    for (int i=2; i<=n; i++) {
       pairs[i] = pairs[i-1] + (i-1)*pairs[i-2];
    }
 
    return pairs[n];
               
}
 
int main() {
    const std::vector<std::pair<int, int>> test_cases = {{3,4}, {4,10}, {5,26}};
    int res;
    for (const auto tc : test_cases) {
      res = solution(tc.first);
      assert(res == tc.second);
      if(DEBUG) {
        std::cout<<"input: "<<tc.first<<"; risultato: "<<res<<"; risultato atteso: "<<tc.second<<std::endl;
      }
    }
}