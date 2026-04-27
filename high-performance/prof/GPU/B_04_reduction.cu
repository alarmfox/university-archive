
// Perform a reduction on a linear array
// Include tracking of clock count per block
// Demonstrate the use of CUDA stream events to track the timing from the host side, in addition to local SM clocks

#include <iostream>

// slower, more general reduction kernel
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

// faster, less general reduction kernel
__global__ void reduction_LessGeneral(float* in, float* out, int size, clock_t* tm) {
		
	if (threadIdx.x==0) tm[blockIdx.x] = clock();

	int x = 2*blockDim.x*blockIdx.x + threadIdx.x;
	int tx = threadIdx.x;
		
	for( int stride=blockDim.x; stride>=1; stride >>=1 ) {  // here we assume that size is a multiple of 2*blockDim.x, which in turn is a power of 2
		__syncthreads();
		if ( (tx<stride) ) {
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
	float out_h[num_blocks];
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

	cudaMemcpy(in_d, in_h, sizeof(float)*SIZE, cudaMemcpyHostToDevice);

    reduction<<<num_blocks, THREADSPERBLOCK>>>(in_d, out_d, SIZE, tm_d);
    //reduction_LessGeneral<<<num_blocks, THREADSPERBLOCK>>>(in_d, out_d, SIZE, tm_d);  // faster, less general (SIZE must be multiple of 2*THREADSPERBLOCK)
	
	cudaMemcpy(out_h, out_d, sizeof(float)*num_blocks, cudaMemcpyDeviceToHost);	
	cudaMemcpy(tm_h, tm_d, sizeof(clock_t)*(num_blocks*2), cudaMemcpyDeviceToHost);
	
	cudaEventRecord( stop, 0 );
	cudaEventSynchronize( stop );
	cudaEventElapsedTime( &streamElapsedTime, start, stop );
	cudaEventDestroy( start );
	cudaEventDestroy( stop );

	float red = 0.0;
	for( int i=0; i<num_blocks; i++){
		red += out_h[i];
	}
	
	std::cout << "\nReduction value:  " << red;
	std::cout << "\n";

	double avgClockCount = 0.0;
    for (int i=0; i<num_blocks; i++){
        avgClockCount += (double)( tm_h[i+num_blocks]-tm_h[i] );
    }
    avgClockCount /= num_blocks;
    std::cout << "\nAverage clock count per block: " << avgClockCount;

	avgClockCount = 0.0;
    for (int i=0; i<num_blocks-1; i++){
        avgClockCount += (double)( tm_h[i+num_blocks]-tm_h[i] );
    }
    avgClockCount /= (num_blocks-1);
    std::cout << "\nAverage clock count per block (inner): " << avgClockCount << "  -  clock count, last block: " << (double)(tm_h[2*num_blocks-1]-tm_h[num_blocks-1]);
	std::cout << "\n";
	
	std::cout << "\nCUDA stream elapsed time:  " << streamElapsedTime;
	
	cudaFree(tm_d);
	cudaFree(out_d);
	cudaFree(in_d);
  
    return 0;
}
