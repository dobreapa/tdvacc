//  localizationGlobalization.cpp

#include "window.h"


/*	 Compute standard deviation and mean for given window	*/
void  compute_sd_m_for_w(window &w , double * mean , double * st_dev , int count) ;

/*	Print window information	*/
void print_w(window w);

/*	Compute mean and standard deviation for all data	*/
void compute_mean_stdev(short *data , int count , double *mean , double *st_dev);

/*	Grow window size	*/
void grow_w(window &w , int count, bool &g);

/*	Create window limits from seed	*/
window create_window_from_seed (int seed);

/*	Search for signal s in localization windows	*/
void search_for_s_in_loc(short *data ,int s , window * loc ,int nr_loc , window * rezult , int &contor);

/*	Compute localization windows for given data	*/
void compute_localization (short *data,int count, int channels , double * mean , double * st_dev , window * loc ,  int & nr_loc);

/*	Compute localization windows for given data	*/
void compute_globalization (short *data,int count, int channels, window * loc , int & nr_loc, window * glob , int & nr_glob);