
// Matrix Multiplication - GPU version

#include <iostream>

#define _E(x) { myCheckFunction((x), __FILE__, __LINE__, 1); }
#define _E_CT(x) { myCheckFunction((x), __FILE__, __LINE__, 0); }
inline void myCheckFunction(cudaError_t cd, const char *fl, int ln, int ab){
	if (cd != cudaSuccess){
		  fprintf(stderr,"Error! - File: %s Line: %d Details: %s\n", fl, ln, cudaGetErrorString(cd));
		  if (ab) exit(cd);
	}
}

__global__ void matrixMultiplication( const float* A, const float* B, float* C, size_t Arows, size_t Acols, size_t Bcols ){

    size_t c = blockIdx.x*blockDim.x+threadIdx.x;
    size_t r = blockIdx.y*blockDim.y+threadIdx.y;

    float dp = 0.0;

    if ( (r<Arows) && (c<Bcols) ) {
        for (size_t k=0; k<Acols; k++) {
            dp += A[ Acols*r+k ] * B[ Bcols*k+c ];
        }
    }
    C[ Bcols*r + c ] = dp;  // note: each thread completely computes a distinct element in the output matrix
}


int main(){

	size_t THREADSPERBLOCK = 1024;
	size_t THREADSPERBLOCK_X = 128;
	size_t THREADSPERBLOCK_Y = THREADSPERBLOCK / THREADSPERBLOCK_X; // make sure that these are all 2's powers

	size_t M1cols = 1024*8+13;
	size_t M1rows = 1024*8+13;
	size_t M2cols = 1024*8+13;
	size_t M2rows = M1cols;

	float *M1;
	float *M2;
	float *MO;

	M1 = (float*) malloc( M1rows*M1cols*sizeof(float));
	M2 = (float*) malloc( M2rows*M2cols*sizeof(float));
	MO = (float*) malloc( M1rows*M2cols*sizeof(float));
	
	size_t num_blocks_x = ( M2cols + THREADSPERBLOCK_X - 1 ) / THREADSPERBLOCK_X;
	size_t num_blocks_y = ( M1rows + THREADSPERBLOCK_Y - 1 ) / THREADSPERBLOCK_Y;
	

	float INITVALUE = 0.5;
	
	for( size_t r=0; r<M1rows; r++){
		for( size_t c=0; c<M1cols; c++){
			M1[ r*M1cols+c ] = 1.0;
		}
	}
	for( size_t r=0; r<M2rows; r++){
		for( size_t c=0; c<M2cols; c++){
			M2[ r*M2cols+c ] = INITVALUE;
		}
	}

	cudaEvent_t start, stop;
	float streamElapsedTime;
	_E( cudaEventCreate(&start) );
	_E( cudaEventCreate(&stop) );
	_E( cudaEventRecord( start, 0 ) );

	float *M1_d;
	float *M2_d;
	float *MO_d;
	_E( cudaMalloc((void **)&M1_d,sizeof(float)*(M1rows*M1cols)) );
	_E( cudaMalloc((void **)&M2_d,sizeof(float)*(M2rows*M2cols)) );
	_E( cudaMalloc((void **)&MO_d,sizeof(float)*(M1rows*M2cols)) );
	
	_E( cudaMemcpy( M1_d, M1, sizeof(float)*(M1rows*M1cols), cudaMemcpyHostToDevice ) );
	_E( cudaMemcpy( M2_d, M2, sizeof(float)*(M2rows*M2cols), cudaMemcpyHostToDevice ) );
	
	dim3 gridDims(num_blocks_x,num_blocks_y);
	dim3 blockDims(THREADSPERBLOCK_X,THREADSPERBLOCK_Y);
	matrixMultiplication<<<gridDims,blockDims>>>( M1_d, M2_d, MO_d, M1rows, M1cols, M2cols);
	
	_E( cudaPeekAtLastError() );     // check for invalid arguments
	
	//_E( cudaDeviceSynchronize() ); // ..if no cudaMemcpy or any other synchronizing primitive follows
	_E( cudaMemcpy( MO, MO_d, sizeof(float)*(M1rows*M2cols), cudaMemcpyDeviceToHost ) );
	
	_E( cudaEventRecord( stop, 0 ) );
	_E( cudaEventSynchronize( stop ) );
	_E( cudaEventElapsedTime( &streamElapsedTime, start, stop ) );
	_E( cudaEventDestroy( start ) );
	_E( cudaEventDestroy( stop ) );
	
	_E( cudaFree(M1_d) );
	_E( cudaFree(M2_d) );
	_E( cudaFree(MO_d) );
	
	for (size_t r=0; (r<M1rows)&&(r<10); r++){
		printf("\n");
		for (size_t c=0; (c<M2cols)&&(c<10); c++){
			printf( " %f", MO[r*M2cols+c ]) ;
		}
		if (M2cols>10) printf("  ...");
    }
    if (M1rows>10) printf("\n  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...");

	printf("\nCUDA stream elapsed time:  %f", streamElapsedTime);

	free(M1);
	free(M2);
	free(MO);
	
    return 0;
}
