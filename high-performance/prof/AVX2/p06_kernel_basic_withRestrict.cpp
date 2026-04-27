
#include <immintrin.h>

#define REP 100000
#define SIZE 4096

double vi1[SIZE] __attribute__((aligned (32)));
double vi2[SIZE] __attribute__((aligned (32)));
double voC[SIZE] __attribute__((aligned (32)));
double voA[SIZE] __attribute__((aligned (32)));
	
inline void vectMult( double * __restrict__ p1, double * __restrict__ p2, double * __restrict__ p3, size_t start, size_t end ){
	
    double* pA1 = static_cast<double*>(__builtin_assume_aligned(p1, 16));
    double* pA2 = static_cast<double*>(__builtin_assume_aligned(p2, 16));
    double* pA3 = static_cast<double*>(__builtin_assume_aligned(p3, 16));

    for (size_t i = start; i < end; ++i){
        pA3[i] = pA1[i] * pA2[i];
    }
}


void kernel(){

    for(int j=0; j<REP; j++){
		vectMult( vi1, vi2, voC, 0, SIZE );
	}


}
