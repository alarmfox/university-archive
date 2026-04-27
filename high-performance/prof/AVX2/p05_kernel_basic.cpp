
#include <immintrin.h>

#define REP 100000
#define SIZE 4096

float vi1[SIZE] __attribute__((aligned (32)));
float vi2[SIZE] __attribute__((aligned (32)));
float voC[SIZE] __attribute__((aligned (32)));
float voA[SIZE] __attribute__((aligned (32)));

void kernel(){
	
    for(int j=0; j<REP; j++){
		for(int i=0; i<SIZE; i++){
			voC[i] = vi1[i] + vi2[i];
		}
	}

}
