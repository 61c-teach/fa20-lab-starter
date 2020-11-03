#include <stdio.h>
#include <stdlib.h>
#include "common.h"

/* ***DON'T MODIFY THIS FILE! ONLY MODIFY COMMON.C!*** */

int main(int argc, char* argv[]) {
	printf("Let's generate a randomized array.\n");
	unsigned int vals[NUM_ELEMS];
	long long int reference;
	long long int simd;
	long long int simdu;
	for(unsigned int i = 0; i < NUM_ELEMS; i++) vals[i] = rand() % 256;

	printf("Starting randomized sum.\n");
	printf("Sum: %lld\n", reference = sum(vals));

	printf("Starting randomized unrolled sum.\n");
	printf("Sum: %lld\n", sum_unrolled(vals));

	printf("Starting randomized SIMD sum.\n");
	printf("Sum: %lld\n", simd = sum_simd(vals));
	if (simd != reference) {
		printf("OH NO! SIMD sum %lld doesn't match reference sum %lld!\n", simd, reference);
	}

	printf("Starting randomized SIMD unrolled sum.\n");
	printf("Sum: %lld\n", simdu = sum_simd_unrolled(vals));
	if (simdu != reference) {
		printf("OH NO! SIMD_UNROLLED sum %lld doesn't match reference sum %lld!\n", simdu, reference);
	}
}
