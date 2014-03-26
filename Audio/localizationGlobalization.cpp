#include "localizationGlobalization.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>



/*	 Compute standard deviation and mean for given window	*/
void  compute_sd_m_for_w(window &w , double * mean , double * st_dev , int count)
{
	if (w.end == w.begin || w.end < w.begin)
		printf ("Error - window is not valid\n");

	double  m = 0 , sd = 0;

	m = mean[w.end] - mean[w.begin];
	sd = st_dev[w.end] - st_dev[w.begin];

	//TO DO !!! verificare formule	

	w.m = m;
	w.sd = sd;
	
	return ;
}


/*	Print window information	*/
void print_w(window w)
{
	printf ("\tWindow info : begin = %i ; end = %i ; sd = %f ; m = %f \n", w.begin , w.end , w.sd , w.m);

	return ;
}


/*	Compute mean and standard deviation for all data	*/
void compute_mean_stdev(short *data ,int count , double *mean , double *st_dev)
{
	if (count==0)
		printf ("Error - data is not valid\n");

	double prev_mean = 0 , m = 0 , n = 0 , s = 0 , sd = 0;

	for (int k = 0 ; k < count ; k++)
		{
           	prev_mean = m;
			n = n + 1;
			m = m + (data [k] - m) / n;
			s = s + (data [k] - m) * (data [k] - prev_mean);
			sd = sqrt (s / n);

			mean[k]   = m;
			st_dev[k] = sd;
		}
	return ;
}


/*	Grow window size	*/
void grow_w(window &w  , int count , bool &g)
{
	if (w.begin == 0 )
		{
			if (w.end+2 <=count)
				{
					w.end += 2;
					g = true;
					return;
				}
			else
			{
				g= false;
				return;
			}
			
		}

	if (w.end == count)
		{
			if (w.begin-2 >= 0)
				{
					w.begin -= 2;
					g = true;
					return;
				}
			else
				{
					g=false;
					return;
				}
		}

	if (w.begin > 0 && w.end < count)
	{
		w.begin --;
		w.end ++;
		g = true;
		return;
	}

	
}


/*	Create window limits from seed	*/
window create_window_from_seed (int seed)
{
	window win;

	win.begin = seed;
	win.end = seed +1;
	win.m = 0;
	win.sd = 0;

	return win;
}


/*	Search for signal s in localization windows	*/
void search_for_s_in_loc(short *data ,int s , window * loc ,int nr_loc , window * rezult , int &contor)
{
	int c = 0;

	for (int w = 0 ; w < nr_loc ; w++)//loc
	{
		for (int i = loc[w].begin ; i < loc[w].end ; i++)//data in loc
		{
			if (data[s] == data[i])
			{
				rezult[c] = loc[w];
				c++;
				break;
			}
		}
	}

	contor = c;
}


/*	Compute localization windows for given data	*/
void compute_localization (short *data,int count, int channels , double * mean , double * st_dev , window * loc , int & nr_loc)
{   
	printf ("\t\t compute_localization begin\n");

	/*	Parcurge daca sunt mai multe canale   */
	/*for (int chan = 0 ; chan < channels ; chan ++)  
	for (int k = chan ; k < count ; k+= channels)*/

	compute_mean_stdev(data , count, mean , st_dev);
	/* Verificare compute_mean_stdev */
	/*for (int i = 0 ; i < count ; i++) 
		printf (" data - %d  mean -%d stdev %d \n" , data[i] ,mean[i] , st_dev[i]);*/

	window w_prev = {};
	window w_current = {};
	window w_first = {};

	w_first.begin = 0;
	w_first.end = 1; 
	compute_sd_m_for_w( w_first,  mean , st_dev, count);
	w_prev = w_first ;
	
	int contor = 0;
	for (int seed = 1 ; seed < count ; seed++)
	{
		w_current = create_window_from_seed (seed);

		while (1)
		{
			bool g;

			grow_w (w_current , count , g);

			if ( g == false)/*daca nu a crescut*/
				break;

			compute_sd_m_for_w( w_current,  mean , st_dev, count);
				
			if ( w_current.sd * log( (double)(w_current.end - w_current.begin))  < 
				w_prev.sd    * log( (double)(w_prev.end    - w_prev.begin))     )
			{
				//print_w (w_current);
				
				loc[contor] = w_current;//save current win
				contor++;
				w_prev = w_current;		
				break;
			}
		}
					
	}

	printf ("Found windows : %i\n",contor);
	nr_loc = contor;

	printf ("\t\t compute_localization end\n");
	return ;
} /* compute_localization */

/*	Compute localization windows for given data	*/
void compute_globalization (short *data,int count, int channels, window * loc , int & nr_loc, window * glob , int & nr_glob)//Major design algoritm fail
{
	printf ("\t\t compute_globalization begin\n");


	window * l;
	int cl;
	l = 	(window *)malloc (nr_loc * sizeof(window) );

	window * list1;
	int c1;
	list1 = 	(window *)malloc (nr_loc * sizeof(window) );

	window * list2;
	int c2;
	list2 = 	(window *)malloc (nr_loc * sizeof(window) );

	window max_w;
	int contor;

	for (int s = 0 ; s < count ; s++)//data
	{
		search_for_s_in_loc(data ,s ,loc ,nr_loc ,list1 , c1);
		for (int ss = 0 ; ss < count ; ss++)//data
		{
			search_for_s_in_loc(data ,ss ,loc ,nr_loc ,list2 , c2);

			contor= 0;
			for (int o = 0 ; o <c1; o++  ) //list1
				for (int p = 0 ; p <c2 ; p++) //list2
				{
					if (list1[o].begin == list2[p].begin && list1[o].end == list2[p].end)
					{
						l[contor] = list1[o];
						contor++;
					}
				}

		//printf ("ss: %i ",ss);
				

		}//ss for

		double max = DBL_MIN; //max for window sd
			
		for (int x = 0 ; x <contor; x++  ) //l
		{
				
			if (max < l[x].sd)
			{
				max = l[x].sd;
				max_w = l[x];  //TO DO initializare max_w
			}
								
		}
		free(l);
			
		glob[s] = max_w;

		printf(" pt indice %i  ", s);
		print_w(glob[s]);

		
	}

	printf ("\t\t compute_globalization end\n");
	return;
}

