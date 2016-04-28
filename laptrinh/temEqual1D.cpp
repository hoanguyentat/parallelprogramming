#include <stdio.h>
#include <stdlib.h>

int n = 20;
float Ntime = 100;
float dt = 0.01;
float dx = 0.1;
float D = 0.1;
float *C, *dC;
void print();

void FD(float *C, float *dC){
	float c, l, r;
	for (int i = 0; i < n; ++i)
	{
			c = *(C + i);
			l = (i) ? *(C+ i -1) : 100;
			r = (i==n-1) ? 25 : *(C+ i +1);
			*(dC + i) = D*(l+r-2*c)/(dx*dx);
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
	for (float t = 0; t < Ntime; t+=dt)
	{
		FD(C, dC);
		for (int i = 0; i < n; ++i)
			*(C+i) += dt * (*(dC+i));
		
	}
	print();
	return 0;
}