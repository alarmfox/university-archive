
// Perform a reduction on a linear array
// Further levels of reduction are performed on the GPU instead of the host

#include <iostream>

__global__ void reduction(float* in, float* out, int size, clock_t* tm) {
		
	if (threadIdx.x==0) tm[blockIdx.x] = clock();

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
	
    __syncthreads();
    if (threadIdx.x==0) tm[blockIdx.x+gridDim.x] = clock();
}


int main(){

	int SIZE = 2048*320 +0;
	int THREADSPERBLOCK= 1024;

	float in_h[SIZE];
	int num_blocks = ( SIZE + 2*THREADSPERBLOCK - 1) / (2*THREADSPERBLOCK);
	clock_t tm_h[num_blocks*2];
	
	for(int i=0; i<SIZE; i++){
		in_h[i] = 1.0;
	}
	
	float * in_d;
	float * out_d;
	clock_t * tm_d;

	cudaEvent_t start, stop;
	float streamElapsedTime;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord( start, 0 );
	
	cudaMalloc((void **)&in_d, sizeof(float)*SIZE );  // (..should check for errors..)
	cudaMalloc((void **)&out_d, sizeof(float)*num_blocks );
	cudaMalloc((void **)&tm_d, sizeof(clock_t)*num_blocks*2 );

	double avgClockCount = 0.0;
	int totBlocks = 0;

	/*
	// "ping-pong" cudaMemcpy transfers between host and device:
	float out_h[num_blocks];
	float* in_tmp=in_h;
	do{
		num_blocks = ( SIZE + 2*THREADSPERBLOCK - 1) / (2*THREADSPERBLOCK);
		cudaMemcpy(in_d, in_tmp, sizeof(float)*SIZE, cudaMemcpyHostToDevice);
		reduction<<<num_blocks, THREADSPERBLOCK>>>(in_d, out_d, SIZE, tm_d);
		cudaMemcpy(out_h, out_d, sizeof(float)*num_blocks, cudaMemcpyDeviceToHost);
		in_tmp=out_h;
		cudaMemcpy(tm_h, tm_d, sizeof(clock_t)*(num_blocks*2), cudaMemcpyDeviceToHost);
		for (int i=0; i<num_blocks; i++){
			avgClockCount += (double)( tm_h[i+num_blocks]-tm_h[i] );
		}
		totBlocks += num_blocks;
		SIZE = num_blocks;
	}while(num_blocks>1);
	*/
	
	// in-place swap of device buffers:
	float out_h[1];
	cudaMemcpy(in_d, in_h, sizeof(float)*SIZE, cudaMemcpyHostToDevice);
	do{
		num_blocks = ( SIZE + 2*THREADSPERBLOCK - 1) / (2*THREADSPERBLOCK);
		reduction<<<num_blocks, THREADSPERBLOCK>>>(in_d, out_d, SIZE, tm_d);
		cudaMemcpy(tm_h, tm_d, sizeof(clock_t)*(num_blocks*2), cudaMemcpyDeviceToHost);
		for (int i=0; i<num_blocks; i++){
			avgClockCount += (double)( tm_h[i+num_blocks]-tm_h[i] );
		}
		totBlocks += num_blocks;
		float* swap = in_d;
		in_d=out_d;
		out_d=swap;
		SIZE = num_blocks;
	}while(num_blocks>1);
	cudaMemcpy(out_h, in_d, sizeof(float)*1, cudaMemcpyDeviceToHost);
	
	
	cudaEventRecord( stop, 0 );
	cudaEventSynchronize( stop );
	cudaEventElapsedTime( &streamElapsedTime, start, stop );
	cudaEventDestroy( start );
	cudaEventDestroy( stop );

	float red = out_h[0];
	
	std::cout << "\nReduction value:  " << red;
	std::cout << "\n";

    avgClockCount /= totBlocks;
    std::cout << "\nAverage clock count per block: " << avgClockCount;	
	std::cout << "\nCUDA stream elapsed time:  " << streamElapsedTime;
	
	cudaFree(tm_d);
	cudaFree(out_d);
	cudaFree(in_d);
  
    return 0;
}
