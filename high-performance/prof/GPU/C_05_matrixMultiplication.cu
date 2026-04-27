
// Matrix Multiplication - GPU version with tiling on Shared Memory and overlapped communication/computation
// Row and column sizes must all be multiples of 32*2 in this version

#include <iostream>

#define _E(x) { myCheckFunction((x), __FILE__, __LINE__, 1); }
#define _E_CT(x) { myCheckFunction((x), __FILE__, __LINE__, 0); }
inline void myCheckFunction(cudaError_t cd, const char *fl, int ln, int ab){
	if (cd != cudaSuccess){
		  fprintf(stderr,"Error! - File: %s Line: %d Details: %s\n", fl, ln, cudaGetErrorString(cd));
		  if (ab) exit(cd);
	}
}

__global__ void matrixMultiplication( const float* A, const float* B, float* C, size_t Acols, size_t Bcols ){

#define WSIZE 32

    size_t bx = blockIdx.x;
    size_t by = blockIdx.y;
    size_t tx = threadIdx.x;
    size_t ty = threadIdx.y;

    __shared__ float Ablock[WSIZE][WSIZE];
    __shared__ float Bblock[WSIZE][WSIZE];

    size_t aFrst = (WSIZE*by) * Acols;
    size_t aLast = aFrst + Acols;
    size_t aStep = WSIZE;
    size_t bFrst = WSIZE*bx;
    size_t bStep = WSIZE * Bcols;

    float val = 0.0;

    size_t aIdx = aFrst;
	size_t bIdx = bFrst; 
	while (aIdx<aLast){
        Ablock[ty][tx] = A[aIdx + Acols*ty + tx];
        Bblock[ty][tx] = B[bIdx + Bcols*ty + tx];
        __syncthreads();
        for (size_t k=0; k<WSIZE; ++k) {
            val += Ablock[ty][k] * Bblock[k][tx];
        }
		aIdx += aStep;
		bIdx += bStep;
        __syncthreads();
    }
    size_t cIdx = (WSIZE*by) * Bcols + WSIZE*bx;
    C[ cIdx + Bcols*ty + tx] = val;
}


int main(){

	size_t THREADSPERBLOCK = 1024;  // fixed in this version
	size_t THREADSPERBLOCK_X = 32;  // fixed in this version
	size_t THREADSPERBLOCK_Y = THREADSPERBLOCK / THREADSPERBLOCK_X; // make sure that these are all 2's powers

	size_t M1cols = 1024*16;  // 32's multiples in this version
	size_t M1rows = 1024*16;  // (32*2)'s multiples in this version
	size_t M2cols = 1024*16;  // (32*2)'s multiples in this version
	size_t M2rows = M1cols;

	float *M1;
	float *M2;
	float *MO;

	_E( cudaMallocHost( (void **)&M1,M1rows*M1cols*sizeof(float)) );
	_E( cudaMallocHost( (void **)&M2,M2rows*M2cols*sizeof(float)) );
	_E( cudaMallocHost( (void **)&MO,M1rows*M2cols*sizeof(float)) );
	
	size_t num_blocks_x = ( (M2cols/2) + THREADSPERBLOCK_X - 1 ) / THREADSPERBLOCK_X;
	size_t num_blocks_y = ( (M1rows/2) + THREADSPERBLOCK_Y - 1 ) / THREADSPERBLOCK_Y;
	
	dim3 gridDims(num_blocks_x,num_blocks_y);
	dim3 blockDims(THREADSPERBLOCK_X,THREADSPERBLOCK_Y);

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

	cudaStream_t stream1,stream2;
	_E( cudaStreamCreate(&stream1) );
	_E( cudaStreamCreate(&stream2) );
	
	cudaEvent_t start, stop, M1Trdy, MOLTrdy, M1Brdy, MOLBrdy;
	_E( cudaEventCreate(&start) );
	_E( cudaEventCreate(&stop) );
	_E( cudaEventCreate(&M1Trdy) );
	_E( cudaEventCreate(&MOLTrdy) );
	_E( cudaEventCreate(&M1Brdy) );
	_E( cudaEventCreate(&MOLBrdy) );
	
	float streamElapsedTime;
	_E( cudaEventRecord( start, stream1 ) );

	
	// ..T: Top matrix half,  ..B: Bottom half,  ..L: Left half,  ..R: Right half,   ..LT/..LB/..RT/..RB: Left-Top / Left-Bottom / Right-Top / Right-Bottom  quadrants
	
	float *M1T_d,*M1B_d;
	float *M2L_d,*M2R_d;
	float *MOLT_d,*MORT_d,*MOLB_d,*MORB_d;
	size_t M1Tptc,M1Bptc;
	size_t M2Lptc,M2Rptc;
	size_t MOLTptc,MORTptc,MOLBptc,MORBptc;
	_E( cudaMallocPitch((void **)&M1T_d,&M1Tptc,sizeof(float)*M1cols,(M1rows/2)) );
	_E( cudaMallocPitch((void **)&M1B_d,&M1Bptc,sizeof(float)*M1cols,(M1rows/2)) );
	_E( cudaMallocPitch((void **)&M2L_d,&M2Lptc,sizeof(float)*(M2cols/2),M2rows) );
	_E( cudaMallocPitch((void **)&M2R_d,&M2Rptc,sizeof(float)*(M2cols/2),M2rows) );
	_E( cudaMallocPitch((void **)&MOLT_d,&MOLTptc,sizeof(float)*(M2cols/2),(M1rows/2)) );
	_E( cudaMallocPitch((void **)&MORT_d,&MORTptc,sizeof(float)*(M2cols/2),(M1rows/2)) );
	_E( cudaMallocPitch((void **)&MOLB_d,&MOLBptc,sizeof(float)*(M2cols/2),(M1rows/2)) );
	_E( cudaMallocPitch((void **)&MORB_d,&MORBptc,sizeof(float)*(M2cols/2),(M1rows/2)) );

	// Stream 1
	_E( cudaMemcpy2DAsync( M1T_d, M1Tptc, M1, sizeof(float)*M1cols, sizeof(float)*M1cols, (M1rows/2), cudaMemcpyHostToDevice, stream1 ) );
	_E( cudaEventRecord( M1Trdy, stream1 ) );
	_E( cudaMemcpy2DAsync( M2L_d, M2Lptc, M2, sizeof(float)*M2cols, sizeof(float)*(M2cols/2), M2rows, cudaMemcpyHostToDevice, stream1 ) );
	matrixMultiplication<<<gridDims,blockDims,0,stream1>>>( M1T_d, M2L_d, MOLT_d, M1cols, (M2cols/2) );
	_E( cudaEventRecord( MOLTrdy, stream1 ) );

	_E( cudaMemcpy2DAsync( M1B_d, M1Bptc, M1+M1cols*(M1rows/2), sizeof(float)*M1cols, sizeof(float)*M1cols, (M1rows/2), cudaMemcpyHostToDevice, stream1 ) );
	_E( cudaEventRecord( M1Brdy, stream1 ) );
	matrixMultiplication<<<gridDims,blockDims,0,stream1>>>( M1B_d, M2L_d, MOLB_d, M1cols, (M2cols/2) );
	_E( cudaEventRecord( MOLBrdy, stream1 ) );

	// Stream 2
	_E( cudaMemcpy2DAsync( M2R_d, M2Rptc, M2+(M2cols/2), sizeof(float)*M2cols, sizeof(float)*(M2cols/2), M2rows, cudaMemcpyHostToDevice, stream2 ) );
	
	_E( cudaStreamWaitEvent( stream2, M1Trdy, 0 ) );
	matrixMultiplication<<<gridDims,blockDims,0,stream2>>>( M1T_d, M2R_d, MORT_d, M1cols, (M2cols/2) );

	_E( cudaStreamWaitEvent( stream2, MOLTrdy, 0 ) );
	_E( cudaMemcpy2DAsync( MO, sizeof(float)*M2cols, MOLT_d, sizeof(float)*(M2cols/2), sizeof(float)*(M2cols/2), (M1rows/2), cudaMemcpyDeviceToDevice, stream2 ) );
	_E( cudaMemcpy2DAsync( MO+(M2cols/2), sizeof(float)*M2cols, MORT_d, sizeof(float)*(M2cols/2), sizeof(float)*(M2cols/2), (M1rows/2), cudaMemcpyDeviceToDevice, stream2 ) );

	_E( cudaStreamWaitEvent( stream2, M1Brdy, 0 ) );
	matrixMultiplication<<<gridDims,blockDims,0,stream2>>>( M1B_d, M2R_d, MORB_d, M1cols, (M2cols/2) );
	
	_E( cudaStreamWaitEvent( stream2, MOLBrdy, 0 ) );
	_E( cudaMemcpy2DAsync( MO+(M2cols*M1rows/2), sizeof(float)*M2cols, MOLB_d, sizeof(float)*(M2cols/2), sizeof(float)*(M2cols/2), (M1rows/2), cudaMemcpyDeviceToDevice, stream2 ) );
	_E( cudaMemcpy2DAsync( MO+(M2cols*M1rows/2)+(M2cols/2), sizeof(float)*M2cols, MORB_d, sizeof(float)*(M2cols/2), sizeof(float)*(M2cols/2), (M1rows/2), cudaMemcpyDeviceToDevice, stream2 ) );
	
	_E( cudaEventRecord( stop, stream1 ) );

	// synchronize Host
	_E( cudaEventSynchronize( stop ) );
	
	_E( cudaEventElapsedTime( &streamElapsedTime, start, stop ) );

	_E( cudaEventDestroy(MOLBrdy) );
	_E( cudaEventDestroy(M1Brdy) );
	_E( cudaEventDestroy(MOLTrdy) );
	_E( cudaEventDestroy(M1Trdy) );
	_E( cudaEventDestroy( start ) );
	_E( cudaEventDestroy( stop ) );
	
	_E( cudaStreamDestroy(stream2) );
	_E( cudaStreamDestroy(stream1) );
	
	_E( cudaFree(MORB_d) );
	_E( cudaFree(MOLB_d) );
	_E( cudaFree(MORT_d) );
	_E( cudaFree(MOLT_d) );
	_E( cudaFree(M2R_d) );
	_E( cudaFree(M2L_d) );
	_E( cudaFree(M1B_d) );
	_E( cudaFree(M1T_d) );
	
	for (size_t r=0; (r<M1rows)&&(r<10); r++){
		printf("\n");
		for (size_t c=0; (c<M2cols)&&(c<10); c++){
			printf( " %f", MO[r*M2cols+c ]) ;
		}
		if (M2cols>10) printf("  ...");
    }
    if (M1rows>10) printf("\n  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...");

	printf("\nCUDA stream elapsed time:  %f", streamElapsedTime);
	
	_E( cudaFreeHost(M1) );
	_E( cudaFreeHost(M2) );
	_E( cudaFreeHost(MO) );
	
    return 0;
}
