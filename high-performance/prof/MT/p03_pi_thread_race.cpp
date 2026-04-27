

#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>


using namespace std;

long long int STEPS = 1<<20;

// in this version, concurrent unsynchronized access to 'sum' will cause a data race:
double sum = 0;

void pi(int start, int count, double dx) {

	for(long long int i=start; i<(start+count); i++){	
		double x = (i+0.5)*dx;
		double tmp = 4.0/(1+x*x);
		sum = sum + tmp;
	}	
	
	return;	
}

int main() {
	
	double dx = 1.0 / STEPS;

	std::clock_t cpuT = std::clock();

	thread t1 ( pi,       0, STEPS/2, dx);
	thread t2 ( pi, STEPS/2, STEPS/2, dx);

	t1.join();
	t2.join();
	
	double pi = sum * dx;

	double pT = (std::clock() - cpuT) / (double)CLOCKS_PER_SEC;
	std::cout << "Finished in " << pT << " seconds [CPU Clock] " << std::endl;
	
	
	std::cout << "\n pi: " << pi;
	
	return 0;
}


