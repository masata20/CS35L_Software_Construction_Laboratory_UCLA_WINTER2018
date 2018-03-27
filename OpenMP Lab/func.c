#include "func.h"
#include "util.h"

void func0(double *weights, double *arrayX, double *arrayY, int xr, int yr, int n)
{
	int i;
	double div = 1/((double)(n));
	#pragma omp parallel for num_threads(4) private(i)
	for(i = 0; i < n; i++){
		weights[i] = div;
		arrayX[i] = xr;
		arrayY[i] = yr;
	}
}

void func1(int *seed, int *array, double *arrayX, double *arrayY,
			double *probability, double *objxy, int *index,
			int Ones, int iter, int X, int Y, int Z, int n)
{
	int i, j;
   	int index_X, index_Y;
	int max_size = X*Y*Z;

	#pragma omp parallel for num_threads(4) private(i)
   	for(i = 0; i < n; i++){
   		arrayX[i] += 1 + 5*rand2(seed, i);
   		arrayY[i] += -2 + 2*rand2(seed, i);
   	}

	int Y_times_Z = Y*Z; 
	double double_ones = (double)Ones;

	#pragma omp parallel for num_threads(4) private(i,j,index_X,index_Y)
   	for(i = 0; i<n; i++){
		double rounded_X = round(arrayX[i]);
		double rounded_Y = round(arrayY[i]); 
   		for(j = 0; j < Ones; j++){
   			index_X = rounded_X + objxy[j*2 + 1];
   			index_Y = rounded_Y+ objxy[j*2];
   			index[i*Ones + j] = fabs(index_X*Y_times_Z + index_Y*Z + iter);
   			if(index[i*Ones + j] >= max_size)
   				index[i*Ones + j] = 0;
   		}
	
		double prob_i = 0;
   		for(j = 0; j < Ones; j++) {
   			prob_i += (pow((array[index[i*Ones + j]] - 100),2) -
   							  pow((array[index[i*Ones + j]]-228),2))/50.0;
   		}
   		probability[i] = prob_i/double_ones;
   	}
}

void func2(double *weights, double *probability, int n)
{
	int i;
	double sumWeights=0;
	
	#pragma omp parallel for num_threads(4) private(i)
	for(i = 0; i < n; i++) 
   		weights[i] = weights[i] * exp(probability[i]);

	#pragma omp parallel for num_threads(4) private(i) reduction(+:sumWeights)
	for(i = 0; i < n; i++)
   		sumWeights += weights[i];

	#pragma omp parallel for num_threads(4) private(i)
	for(i = 0; i < n; i++)
   		weights[i] = weights[i]/sumWeights;
}

void func3(double *arrayX, double *arrayY, double *weights, double *x_e, double *y_e, int n)
{
	double estimate_x=0.0;
	double estimate_y=0.0;
    int i;

	#pragma omp parallel for num_threads(4) private(i) reduction(+:estimate_x, estimate_y)
	for(i = 0; i < n; i++){
		double weight = weights[i];
   		estimate_x += arrayX[i] * weight;
   		estimate_y += arrayY[i] * weight;
   	}

	*x_e = estimate_x;
	*y_e = estimate_y;

}

void func4(double *u, double u1, int n)
{
	int i;
	double n_double = (double)n;

	#pragma omp parallel for num_threads(4) private(i) 
	for(i = 0; i < n; i++){
   		u[i] = u1 + i/n_double;
   	}
}

void func5(double *x_j, double *y_j, double *arrayX, double *arrayY, double *weights, double *cfd, double *u, int n)
{
	int i, j;
	int n_sub_1 = n-1;
	double div_n = 1 / ((double)(n));

	#pragma omp parallel for num_threads(4) private(i,j)
	for(j = 0; j < n; j++){
   		//i = findIndex(cfd, n, u[j]);
   		i = findIndexBin(cfd, 0, n, u[j]);
   		if(i == -1)
   			i = n_sub_1; 
   		x_j[j] = arrayX[i];
   		y_j[j] = arrayY[i];

   	}

	#pragma omp parallel for num_threads(4) private(i)
	for(i = 0; i < n; i++){
		arrayX[i] = x_j[i];
		arrayY[i] = y_j[i];
		weights[i] = div_n;
	}
}
