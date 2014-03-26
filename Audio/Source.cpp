#include "localizationGlobalization.h"
#include "adaptiveAmplification.h"
#include "noiseReduction.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>
#include <math.h>
#include <time.h> 

using namespace std;

#define MAX_CHANNELS    6

int main (void)
{   
	short *data;
	double *mean , *st_dev ;
		

    SNDFILE      *infile, *outfile_nr , *outfile_aa ; /* input , output for noise reduction , output for adaptive amplification*/
    SF_INFO      sfinfo ;
    int          readcount ;


    if (! (infile = sf_open ("adios.wav",SFM_READ, &sfinfo)))
    {   /* Open failed so print an error message. */
        printf ("Not able to open input file %s.\n", "input.wav") ;
        /* Print the error message fron libsndfile. */
        sf_perror (NULL) ;
        return  1 ;
        } ;

	if (sfinfo.channels > MAX_CHANNELS)
    {   printf ("Not able to process more than %d channels\n", MAX_CHANNELS) ;
        return  1 ;
        } ;
	
	int size = sfinfo.frames * sfinfo.channels;
	int in_frames = sfinfo.frames;
	data = (short*) malloc(size*sizeof (short));					
	mean = (double*)   malloc (in_frames * sizeof (double)  );
	st_dev = (double*) malloc (in_frames * sizeof (double)  );

	window *loc;
	int nr_loc;
	loc = (window *)malloc (in_frames * sizeof(window) );

	for (int i = 0 ; i < in_frames ; i++)
	{
		mean[i] = 0;
		st_dev[i] = 0;
	}

	printf ("Input has %d channels\n", sfinfo.channels);
	printf ("Input has %i frames\n\n",sfinfo.frames);
	

    if (! (outfile_nr = sf_open ("output_nr.wav",SFM_WRITE, &sfinfo)))
    {   printf ("Not able to open output for noise reduction file %s.\n", "output_nr.wav") ;
        sf_perror (NULL) ;
        return  1 ;
        } ;

	 if (! (outfile_aa = sf_open ("outfile_aa.wav",SFM_WRITE, &sfinfo)))
    {   printf ("Not able to open output for adaptive amplification file %s.\n", "outfile_aa.wav") ;
        sf_perror (NULL) ;
        return  1 ;
        } ;

/* /////////////////////////Localization/////////////////////////*/
	clock_t t;
	t = clock();
	/* While there are samples in the input file, read them and process them */
	while ((readcount = sf_readf_short (infile, data, in_frames)))
    {   
		compute_localization (data, readcount, sfinfo.channels , mean , st_dev, loc , nr_loc) ;

		/** Median Filter **/
		medianfilter(data, readcount, sfinfo.channels , loc , nr_loc);
		sf_writef_short (outfile_nr, data, readcount) ;
    } ;

	for(int i=0; i<nr_loc; i++){
		print_w(loc[i]);
	}

	t = clock() - t;
	printf ("Compute time for localization : %.2f seconds\n",((float)t)/CLOCKS_PER_SEC);
		
	/*for (int i = 0 ; i < 10 ; i++)//Verificare localization
		print_w(loc[i]);*/


/* /////////////////////////End Localization/////////////////////////*/



/* /////////////////////////Noise reduction/////////////////////////*/

	t = clock();
	
    /* While there are samples in the input file, read them ,process them and write the result in output*/
	//while ((readcount = sf_readf_short (infile, data, in_frames)))
    //{   
		//compute_noise_reduction(data, readcount, sfinfo.channels , loc , nr_loc) ;
		
    //} ;

	t = clock() - t;
	printf ("Compute time for Noise reduction : %.4f seconds\n",((float)t)/CLOCKS_PER_SEC);

/* /////////////////////////End Noise reduction/////////////////////////*/



/* /////////////////////////Adaptive amplification/////////////////////////*/
	t = clock();
	
    /* While there are samples in the input file, read them, process them and write the result in output*/
	while ((readcount = sf_readf_short (infile, data, in_frames)))
    {   
		compute_adaptive_amplification (data, readcount, sfinfo.channels , loc , nr_loc) ;
		sf_writef_short (outfile_aa, data, readcount) ;
        } ;

	t = clock() - t;
	printf ("Compute time for Adaptive amplification : %.4f seconds\n",((float)t)/CLOCKS_PER_SEC);

/* /////////////////////////End Adaptive amplification/////////////////////////*/



    /* Close input and output files. */
    sf_close (infile) ;
    sf_close (outfile_nr) ;
	sf_close (outfile_aa) ;

	printf("\n\n\n");
	system("pause");
    return 0 ;
} /* main */
