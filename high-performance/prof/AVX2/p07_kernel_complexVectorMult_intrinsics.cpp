
#include <immintrin.h>

#define REP 100000
#define SIZE 2*4096

double vinput1[SIZE] __attribute__((aligned (32)));
double vinput2[SIZE] __attribute__((aligned (32)));
double voutput[SIZE] __attribute__((aligned (32)));
	
inline void complexArrayMult( double * __restrict__ p1, double * __restrict__ p2, double * __restrict__ p3, size_t size ){
	
    __m256d NC = _mm256_setr_pd(1.0, -1.0, 1.0, -1.0);

	double* pA1 = static_cast<double*>(__builtin_assume_aligned(p1, 32));
    double* pA2 = static_cast<double*>(__builtin_assume_aligned(p2, 32));
    double* pA3 = static_cast<double*>(__builtin_assume_aligned(p3, 32));

    for (size_t i = 0; i < size; i+=4){
		
		__m256d v1 = _mm256_load_pd(pA1);    // load four doubles (two complex numbers)
		__m256d v2 = _mm256_load_pd(pA2);
	  
		__m256d v3 = _mm256_mul_pd(v1, v2); // component-wise multiplication
		v2 = _mm256_permute_pd(v2, 0x5);    // swap real and imaginary coefficients in the second element. 0x5 = 0101b: for each half, double in position 0 goes to 1 and v.v.
		v2 = _mm256_mul_pd(v2, NC);         // negate imaginary coefficients
		__m256d v4 = _mm256_mul_pd(v1, v2); // crossed component-wise multiplicatio
		v1 = _mm256_hsub_pd(v3, v4);        // form the resulting components of the complex product
		
		_mm256_store_pd(pA3,v1);            // store four doubles (two complex numbers)
		
		pA1 +=4;
		pA2 +=4;
		pA3 +=4;
    }
}


void kernel(){

    for(int j=0; j<REP; j++){
		complexArrayMult( vinput1, vinput2, voutput, SIZE );
	}
	
}
