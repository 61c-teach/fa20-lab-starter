#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#include "omp_apps.h"

int main() {
	double *x = gen_array(ARRAY_SIZE);
	double *y = gen_array(ARRAY_SIZE);
	double serial_result = 0.0;
	double result;

	double start_time, run_time;

	// calculate result serially
	for(int i=0; i<ARRAY_SIZE; i++)
		serial_result += x[i] * y[i];

	int num_threads = omp_get_max_threads();

	// Test framework that sweeps the number of threads and times each ru
	for (int i=1; i<=num_threads; i++) {
		omp_set_num_threads(i);
		start_time = omp_get_wtime();
		for(int j=0; j<REPEAT; j++) {
			result = dotp_manual_optimized(x, y, ARRAY_SIZE);
		}
		run_time = omp_get_wtime() - start_time;

		// verify result is correct (within some threshold)
		if (fabs(serial_result - result) > 0.001) {
			printf("Manual optimized does not match reference.\n");
			return -1;
		}

		printf("Manual Optimized: %d thread(s) took %f seconds\n",i,run_time);
	}

	for (int i=1; i<=num_threads; i++) {
		omp_set_num_threads(i);
		start_time = omp_get_wtime();
		for(int j=0; j<REPEAT; j++) {
		  result = dotp_reduction_optimized(x, y, ARRAY_SIZE);
		}
		run_time = omp_get_wtime() - start_time;

		// verify result is correct (within some threshold)
		if (fabs(serial_result - result) > 0.001) {
		  printf("Reduction optimized does not match reference.\n");
		  return -1;
		}

		printf("Reduction Optimized: %d thread(s) took %f seconds\n",i,run_time);
	}

	// Only run this once because it's too slow..
	omp_set_num_threads(1);
	start_time = omp_get_wtime();
	for(int j=0; j<REPEAT; j++) {
		result = dotp_naive(x, y, ARRAY_SIZE);
	}
	run_time = omp_get_wtime() - start_time;
	printf("Naive: 1 thread took %f seconds\n",run_time);

	return 0;
}
