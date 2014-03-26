// noiseReduction.cpp

#include "window.h"

/*	Compute noise reduction for given data	*/
void compute_noise_reduction(short *data,int count, int channels , window * loc ,  int & nr_loc);

/** Compare function fo qsort **/
int cmpfunc (const void * a, const void * b);

/** Median Filter **/
void medianfilter(short *data, int count, int channels , window * loc ,  int & nr_loc);