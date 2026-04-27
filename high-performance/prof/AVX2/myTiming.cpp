
#include <windows.h>

static LARGE_INTEGER frequency;
static LARGE_INTEGER start;
static LARGE_INTEGER stop;
static int init = 0;

void timingInit(){
    QueryPerformanceFrequency(&frequency);
}

void timingStart(){
    if (!init) { QueryPerformanceFrequency(&frequency); init=1; }
    QueryPerformanceCounter(&start);
}

void timingStop(){
    if (!init) { QueryPerformanceFrequency(&frequency); init=1; }
    QueryPerformanceCounter(&stop);
}

long long int timingGetCycleDelta(){
    return (stop.QuadPart - start.QuadPart);
}

double timingGetSecondDelta(){
    return ( (static_cast<double>(stop.QuadPart - start.QuadPart)) / static_cast<double>(frequency.QuadPart) );
}

long long int timingGetFrequency(){
    return frequency.QuadPart;
}

