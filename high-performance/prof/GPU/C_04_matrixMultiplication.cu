
// Matrix Multiplication - GPU version with tiling on Shared Memory
// Note: for keeping the code simple, row and column sizes must all be 32's multiples in this version

#include <iostream>

#define _E(x) { myCheckFunction((x), __FILE__, __LINE__, 1); }
#define _E_CT(x) { myCheckFunction((x), __FILE__, __LINE__, 0); }
inline void myCheckFunction(cudaError_t cd, const char *fl, int ln, int ab){
	if (cd != cudaSuccess){
		  fprintf(stderr,"Error! - File: %s Line: %d Details: %s\n", fl, ln, cudaGetErrorString(cd));
		  if (ab) exit(cd);
	}
}

__global__ void matrixMultiplication( const float* A, const float* B, float* C, int Acols, int Bcols ){
// Acols: number of columns of the whole matrix A
// Bcols: number of columns of the whole matrix B

// A and B block sizes are defined separately for clarity, but they must all be equal (i.e. blocks in C are equal to blocks in A and B)
#define Abcols 32
#define Abrows 32
#define Bbcols 32
#define Bbrows Abcols

    int bx = blockIdx.x;  // block coordinates (referring to output matrix C)
    int by = blockIdx.y;
    int tx = threadIdx.x; // relative element coordinates within a block (referring to output matrix C)
    int ty = threadIdx.y;

    __shared__ float Ablock[Abrows][Abcols];
    __shared__ float Bblock[Bbrows][Bbcols];

    int aFrst = (Abrows*by) * Acols; // Beginning of a row (and block) in linearized matrix A
    int aLast = aFrst + Acols;       // Last address along A's row
    int aStep = Abcols;              // Step to the next block along a row in A
    int bFrst = Bbcols*bx;           // Beginning of a block in the first row of linearized matrix B
    int bStep = Bbrows * Bcols;      // Step to the next block along a column in B

    float val = 0.0;

    int aIdx = aFrst;
	int bIdx = bFrst; 
	while (aIdx<aLast){
        
        Ablock[ty][tx] = A[aIdx + Acols*ty + tx];  // note: this copy can only work if Abcols==Abrows==Bbcols==Bbrows (i.e. blocks in C are equal to blocks in A and B)
        Bblock[ty][tx] = B[bIdx + Bcols*ty + tx];

        __syncthreads();

        for (int k=0; k<Abcols; ++k) {
            val += Ablock[ty][k] * Bblock[k][tx];
        }
		aIdx += aStep;
		bIdx += bStep;
		
        __syncthreads();
    }

    int cIdx = (Abrows*by) * Bcols + Bbcols*bx;
    C[ cIdx + Bcols*ty + tx] = val;
}


int main(){

	int THREADSPERBLOCK = 1024;  // fixed in this version
	int THREADSPERBLOCK_X = 32;  // fixed in this version
	int THREADSPERBLOCK_Y = THREADSPERBLOCK / THREADSPERBLOCK_X; // make sure that these are all 2's powers

	int M1cols = 1024*16;  // 32's multiples in this version
	int M1rows = 1024*16;  // 32's multiples in this version
	int M2cols = 1024*16;  // 32's multiples in this version
	int M2rows = M1cols;

	float *M1;
	float *M2;
	float *MO;

	M1 = (float*) malloc( M1rows*M1cols*sizeof(float));
	M2 = (float*) malloc( M2rows*M2cols*sizeof(float));
	MO = (float*) malloc( M1rows*M2cols*sizeof(float));
	
	int num_blocks_x = ( M2cols + THREADSPERBLOCK_X - 1 ) / THREADSPERBLOCK_X;
	int num_blocks_y = ( M1rows + THREADSPERBLOCK_Y - 1 ) / THREADSPERBLOCK_Y;
	

	float INITVALUE = 0.5;
	
	for( int r=0; r<M1rows; r++){
		for( int c=0; c<M1cols; c++){
			M1[ r*M1cols+c ] = 1.0;
		}
	}
	for( int r=0; r<M2rows; r++){
		for( int c=0; c<M2cols; c++){
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
	matrixMultiplication<<<gridDims,blockDims>>>( M1_d, M2_d, MO_d, M1cols, M2cols);
	
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
	
	for (int r=0; (r<M1rows)&&(r<10); r++){
		printf("\n");
		for (int c=0; (c<M2cols)&&(c<10); c++){
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
