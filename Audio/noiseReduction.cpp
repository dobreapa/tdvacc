#include "noiseReduction.h"
#include "localizationGlobalization.h"

#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;


/*	Compute noise reduction for given data	*/
void compute_noise_reduction(short *data,int count, int channels , window * loc ,  int & nr_loc)
{
	printf ("\t\t noise_reduction begin\n");
	//	Parcurge daca sunt mai multe canale   
	//for (int chan = 0 ; chan < channels ; chan ++)  
	//for (int k = chan ; k < count ; k+= channels)

	for (int i = 0 ; i < count ; i++)
	{
		//process data[i]
		//TO DO 
	}

	printf ("\t\t compute_noise_reduction end\n");
	return;
}

/** Compare function fo qsort **/
int cmpfunc (const void * a, const void * b)
{
   return ( *(short*)b - *(short*)a );
}


/** Median Filter **/
void medianfilter(short *data, int count, int channels , window * loc ,  int & nr_loc)
{
   printf ("\t\t median_filter begin\n");
   short *aux;
   

   for(int i = 0; i < nr_loc; i++){
	   aux = (short*) malloc((loc[i].end - loc[i].begin)*sizeof (short));
	   int contor = 0;
	   for(int j = loc[i].begin; j < loc[i].end; j++){
		   aux[contor] = data[j];
		   contor++;
	   }

	   //sortare
	   qsort(aux, loc[i].end - loc[i].begin, sizeof(short), cmpfunc);
	   
	   int size = (int)((loc[i].end - loc[i].begin) / 8);
	   
	   short max = (short)aux[size];
	   short min = (short)aux[size*7];
	   
	   //printf("\nWindow: %i, Size: %i, Min: %i, Max: %i\n", i, size, min, max);

	   for(int j = loc[i].begin; j < loc[i].end; j++){
		   if(data[j] < min || (max != 0 && data[j] >= max)){
			   //TO DO -> eliminare
			   data[j] = 0;
		   }
	   }
	   
	   free(aux);
   }

   

   printf ("\t\t median_filter end\n");
   return;
}