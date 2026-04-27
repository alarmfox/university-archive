

#include <iostream>
#include <ctime>
#include <chrono>

using namespace std;

long long int STEPS = 1<<24;

double sum = 0;

void pi(int start, int count, double dx) {

	for(long long int i=start; i<(start+count); i++){	
		double x = (i+0.5)*dx;
		sum = sum + 4.0/(1+x*x);
	}	
		
}

// sequential version with timing:

int main() {
	
	double dx = 1.0 / STEPS;

	std::clock_t cpuT = std::clock();

	pi (      0, STEPS/2, dx);
	
	pi (STEPS/2, STEPS/2, dx);
	
	double pi = sum * dx;

	double pT = (std::clock() - cpuT) / (double)CLOCKS_PER_SEC;
	std::cout << "Finished in " << pT << " seconds [CPU Clock] " << std::endl;
	
	
	std::cout << "\n pi: " << pi;
	
	return 0;
}


