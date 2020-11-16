
#ifndef COMPUTE_DOTP_H
#define COMPUTE_DOTP_H

#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libbmp/libbmp.h"

#define REPEAT 60
#define BUF_SIZE 8192

#define ARRAY_SIZE 5000000

// for web server
char *compute_dotp(int);
char *image_proc(const char*);

void v_add_naive(double* x, double* y, double* z);
void v_add_optimized_adjacent(double* x, double* y, double* z);
void v_add_optimized_chunks(double* x, double* y, double* z);
double dotp_naive(double* x, double* y, int arr_size);
double dotp_manual_optimized(double* x, double* y, int arr_size);
double dotp_reduction_optimized(double* x, double* y, int arr_size);

double* gen_array(int n);
int verify(double* x, double* y, void(*funct)(double *x, double *y, double *z));

#endif