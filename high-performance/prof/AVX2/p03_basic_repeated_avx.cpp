
#include <iostream>
#include <chrono>

#include <immintrin.h>

#include <myTiming.h>

#define REP 12500
#define SIZE 4096

int main(){

	float vi1[SIZE];
	float vi2[SIZE];
	float voC[SIZE];
	float voA[SIZE];

	
	// std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	
	//LARGE_INTEGER frequency;
    //QueryPerformanceFrequency(&frequency);
    //LARGE_INTEGER start;
    //QueryPerformanceCounter(&start);

	timingStart();
	
    for(int j=0; j<REP; j++){
		for(int i=0; i<SIZE; i++){
			voC[i] = vi1[i] * vi2[i];
		}
	}
		
	timingStop();	
		
	//std::cout << timingGetCycleDelta() << " cycles (" << timingGetSecondDelta() << " seconds, frequency: " << timingGetFrequency() << "Hz)";

	//
	printf("\n%lld cycles (%lf seconds, frequency: %lldHz)", timingGetCycleDelta(), timingGetSecondDelta(), timingGetFrequency() );
		
    //std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    //std::cout << "It took me " << time_span.count() << " seconds.";

    //LARGE_INTEGER end;
    //QueryPerformanceCounter(&end);
    //// for microseconds use 1000000.0
    //double interval = static_cast<double>(end.QuadPart- start.QuadPart) / frequency.QuadPart; // in seconds
    //std::cout << (static_cast<double>(end.QuadPart- start.QuadPart)) << " cycles (" << interval << " seconds, frequency: " << (frequency.QuadPart) << "Hz)";

    return 0;
}
