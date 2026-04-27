
#include <iostream>
#include <windows.h>

void kernel();

int main() {

  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);

  LARGE_INTEGER start, stop;

  QueryPerformanceCounter(&start);

  kernel();

  QueryPerformanceCounter(&stop);

  double interval =
      static_cast<double>(stop.QuadPart - start.QuadPart) / frequency.QuadPart;
  std::cout << "\n\n"
            << (stop.QuadPart - start.QuadPart) << " cycles (" << interval
            << " seconds, frequency: " << (frequency.QuadPart) << "Hz)";

  return 0;
}
