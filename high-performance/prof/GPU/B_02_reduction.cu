
// Perform a reduction on a linear array
// Threads in a block now correctly synchronize.
// Thread blocks process 2*blockDim.x elements in the input array.
// SIZE can be any value, including odd numbers.

#include <iostream>

__global__ void reduction(float* in, float* out, int size) {
		
	int x = 2*blockDim.x*blockIdx.x + threadIdx.x;
	int tx = threadIdx.x;
		
	int stride = 2*blockDim.x;
	if ( (2*blockDim.x*(blockIdx.x+1))>size ){
		stride = size%(2*blockDim.x);
	}
	
	while( stride>1 ) {
		int odd = stride%2;  // equal to 1 if we are adding an odd number of elements
		stride = (stride+1)/2;
		__syncthreads();
		if ( (tx<stride) && ((tx!=stride-1)||(odd==0)) ) {
			in[x] += in[x+stride];
		}		
	}
	if (tx==0) {
		out[blockIdx.x] = in[x];
	}
}


int main(){

	int SIZE = 2048*320;
	int THREADSPERBLOCK= 1024;

	float in_h[SIZE];
	int num_blocks = ( SIZE + 2*THREADSPERBLOCK - 1) / (2*THREADSPERBLOCK);
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
