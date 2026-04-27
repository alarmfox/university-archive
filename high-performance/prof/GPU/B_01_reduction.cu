
// Perform a reduction on a linear array - WRONG VERSION (thread synchronization missing in thread blocks)
// This version is also unoptimized (half of the threads are wasted in the first iteration of the reduction loop)
// Furthermore, SIZE must be a multiple of THREADSPERBLOCK

#include <iostream>

__global__ void reduction(float* in, float* out, int size) {
		
	int x = blockDim.x*blockIdx.x + threadIdx.x;
	int tx = threadIdx.x;
		
	for(int stride=blockDim.x/2; stride>=1; stride>>=1) {  // note: blockDim.x, of course, is assumed to be a power of 2
		if ((tx<stride)&&(x+stride<size)){
			in[x] += in[x+stride];
		}
	}
	if (tx==0) {
		out[blockIdx.x] = in[x];
	}
}

int main(){

	int SIZE = 2048*320;
	int THREADSPERBLOCK = 1024;

	float in_h[SIZE];
	int num_blocks = (SIZE + THREADSPERBLOCK - 1) / THREADSPERBLOCK;
	float out_h[num_blocks];
	
	for(int i=0; i<SIZE; i++){
		in_h[i] = 1.0;
	}
	
	float * in_d;
	float * out_d;
	
	cudaMalloc((void **)&in_d, sizeof(float)*SIZE );  // (..should check for errors..)
	cudaMalloc((void **)&out_d, sizeof(float)*num_blocks );

	cudaMemcpy(in_d, in_h, sizeof(float)*SIZE, cudaMemcpyHostToDevice);

    reduction<<<num_blocks, THREADSPERBLOCK>>>(in_d, out_d, SIZE);
	
	cudaMemcpy(out_h, out_d, sizeof(float)*num_blocks, cudaMemcpyDeviceToHost);	
	
	float red = 0.0;
	for( int i=0; i<num_blocks; i++){
		red += out_h[i];
	}
	
	std::cout << "\nReduction value:  " << red;
	std::cout << "\n";

	cudaFree(out_d);
	cudaFree(in_d);
  
    return 0;
}
