#ifndef COMMON_H
#define COMMON_H

#include <x86intrin.h>

#define NUM_ELEMS ((1 << 16) + 10)
#define OUTER_ITERATIONS (1 << 16)

long long int sum(unsigned int vals[NUM_ELEMS]);

long long int sum_unrolled(unsigned int vals[NUM_ELEMS]);

long long int sum_simd(unsigned int vals[NUM_ELEMS]);

long long int sum_simd_unrolled(unsigned int vals[NUM_ELEMS]);

#endif
