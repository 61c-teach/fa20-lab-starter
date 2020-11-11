#ifndef SIMD_H
#define SIMD_H

#include <x86intrin.h>

#define NUM_ELEMS ((1 << 16) + 10)
#define OUTER_ITERATIONS (1 << 14)

long long int sum(int vals[NUM_ELEMS]);

long long int sum_unrolled(int vals[NUM_ELEMS]);

long long int sum_simd(int vals[NUM_ELEMS]);

long long int sum_simd_unrolled(int vals[NUM_ELEMS]);

#endif
