#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define B 10


int n = 20;
int Ntime = 100;
float dt = 0.01;
float dx = 0.1;
float D = 0.1;
float *C, *dC;
void print();

void FD(float *C, float *dC, int start, int stop){
	float c, l, r;
	for (int i = start; i < stop; ++i)
	{
			c = *(C + i);
			l = (i) ? *(C+ i -1) : 100;
			r = (i==n-1) ? 25 : *(C+ i +1);
			*(dC + i) = D*(l+r-2*c)/(dx*dx);
			// printf("%3.2f %3.2f %3.2f %3.2f\n",c, l, r, *(dC+i));
	}	
}


void print(){
	for (int i = 0; i < n; ++i)
		{
			printf("%3.2f ",*(C + i));
		}
	printf("\n");
}

int main(int argc, char *argv[])
{
	dC = (float*) malloc (n * sizeof(float));
	C = (float*) malloc (n * sizeof(float));

	for (int i = 0; i < n; ++i){
		*(dC+i) = 0;
		*(C+i) = 25;	
	}

	
	omp_set_num_threads(B);

	int mc = n/B;
	int id, i, start, stop;
	float t = 0;
	#pragma omp parallel private(id, i, start, stop, t)
	{
		
		id = omp_get_thread_num();
		start = id * mc;
		stop = start + mc;

		while(t <= Ntime)
		{
			#pragma omp barrier
			FD(C, dC, start, stop);
			for (i = start; i < stop; ++i)
				*(C+i) += dt * (*(dC+i));
			t = t + dt;
		}
	}
	print();
	return 0;
}