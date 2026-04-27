
// Add the column elements of a matrix to an output vector - GPU and host versions

#include <iostream>

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

void kernel_host( float mat[ROWS][COLS], float vec[COLS], int rows, int cols ){
	for( int c=0; c<cols; c++){
		vec[c] = 0;
		for( int r=0; r<rows; r++){
			vec[c] += mat[r][c];
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
	
    cudaMalloc((void **)&d_Mat, size_Mat);  // (..should check for errors..)
    cudaMalloc((void **)&d_Vec, size_Vec);

	cudaMemcpy(d_Mat, myMat, size_Mat, cudaMemcpyHostToDevice);

    int threadsPerBlock = 256;
    int blocksPerGrid =(COLS + threadsPerBlock - 1) / threadsPerBlock;
    kernel<<<blocksPerGrid, threadsPerBlock>>>(d_Mat, d_Vec, ROWS, COLS);
	
	cudaMemcpy(myVec, d_Vec, size_Vec, cudaMemcpyDeviceToHost);

	
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

	// --------- host
	
	float myVec_host[COLS];

    kernel_host(myMat, myVec_host, ROWS, COLS);

	std::cout << "\n\n";
	for( int c=0; c<COLS; c++){
		std::cout << myVec_host[c] << "  ";
	}

	std::cout << "\n";
	
	
    return 0;
}

