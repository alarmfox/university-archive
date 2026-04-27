
// Add the column elements of a matrix to an output vector - GPU version with Error Handling

#include <iostream>

// Use the CHKERR macro to check for errors and, in case, abort execution. Use CHKERR_CT to continue execution even in case of errors.
#define CHKERR(x) { myCheckFunction((x), __FILE__, __LINE__, 1); }
#define CHKERR_CT(x) { myCheckFunction((x), __FILE__, __LINE__, 0); }
inline void myCheckFunction(cudaError_t cd, const char *fl, int ln, int ab){
	if (cd != cudaSuccess){
		  fprintf(stderr,"Error! - File: %s Line: %d Details: %s\n", fl, ln, cudaGetErrorString(cd));
		  if (ab) exit(cd);
	}
}

#define ROWS 5
#define COLS 10

__global__ void kernel( float *mat, float *vec, int rows, int cols ){
	
    int c = blockDim.x * blockIdx.x + threadIdx.x;
	
	if (c < cols){
		vec[c] = 0;
		for( int r=0; r<rows; r++){
			vec[c] += mat[r*cols+c];
		}
	}

}

int main(){

	//float myMat[ROWS][COLS] = { {1,2,3,4,5,6,7,8,9,0}, {1,2,3,4,5,6,7,8,9,0}, {1,2,3,4,5,6,7,8,9,0}, {1,2,3,4,5,6,7,8,9,0}, {1,2,3,4,5,6,7,8,9,0} };
	float myMat[ROWS][COLS];
	float myVec[COLS];

	for (int r=0; r<ROWS; r++){
		for (int c=0; c<COLS; c++){
			myMat[r][c] = rand()/(float)RAND_MAX;
		}
    }
	
	int size_Mat = sizeof( float[ROWS][COLS] );
	int size_Vec = sizeof( float[COLS] );
	float *d_Mat = NULL, *d_Vec = NULL;
	
    CHKERR( cudaMalloc((void **)&d_Mat, size_Mat) );
    CHKERR( cudaMalloc((void **)&d_Vec, size_Vec) );

	CHKERR( cudaMemcpy(d_Mat, myMat, size_Mat, cudaMemcpyHostToDevice) );

    int threadsPerBlock = 256;
    int blocksPerGrid =(COLS + threadsPerBlock - 1) / threadsPerBlock;
    kernel<<<blocksPerGrid, threadsPerBlock>>>(d_Mat, d_Vec, ROWS, COLS);
	
	CHKERR( cudaPeekAtLastError() );     // check for invalid arguments
	
	//CHKERR( cudaDeviceSynchronize() ); // the subsequent cudaMemcpy here synchronizes and reports possible kernel's errors (if no synchronizing call follows, explicitly use cudaDeviceSynchronize)
	CHKERR( cudaMemcpy(myVec, d_Vec, size_Vec, cudaMemcpyDeviceToHost) );

	
	for( int r=0; r<ROWS; r++){
		std::cout << "\n";
		for( int c=0; c<COLS; c++){
			std::cout << myMat[r][c] << "  ";
		}
	}
	std::cout << "\n\n";
	for( int c=0; c<COLS; c++){
		std::cout << myVec[c] << "  ";
	}

	std::cout << "\n";
	
    return 0;
}

