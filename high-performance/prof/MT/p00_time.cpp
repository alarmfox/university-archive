#include <iostream>
#include <ctime>
#include <chrono>

// two possible ways of measuring time..

int main() {

  std::clock_t cpuT = std::clock();
  // .. do something ..
  double pT = (std::clock() - cpuT) / (double)CLOCKS_PER_SEC;
  std::cout << "Finished in " << pT << " seconds [CPU Clock] " << std::endl;

  auto sysT = std::chrono::system_clock::now();
  // .. do something ..
  std::chrono::duration<double> pT2 = (std::chrono::system_clock::now() - sysT);
  std::cout << "Finished in " << pT2.count() << " seconds [Wall Clock]"
            << std::endl;
}
