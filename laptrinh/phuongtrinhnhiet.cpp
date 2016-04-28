#include <stdio.h>
#include <stdlib.h>

float t = 0; 
float T = 2;
float dt = 0.01;
float dx = 0.1;
float D = 0.1;
float *C, *dC;
void readFile();
void print(int);
int m, n;
// int m = 4, n = 4;

void FD(float *C, float *dC){
	float c, u, d, l, r;
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			c = *(C + i*n +j);
			u = (i) ? *(C+(i-1)*n+j) : *(C+i*n+j);
			d = (i== m-1) ? *(C+i*n+j) : *(C+(i+1)*n+j);
			l = (j) ? *(C+i*n + j-1) : *(C+i*n + j);
			r = (j==n-1) ? *(C+i*n + j) : *(C+i*n + j+1);
			*(dC + i*n +j) = (1/(dx*dx))*(u +d+l+r-4*c);
		}
	}	
}

void readFile(){
	int count;
	FILE *f = fopen("file.txt","rt");
	if (f == NULL)
	{
		printf("Khong mo duoc file\n");
		exit(1);
	}
	rewind(f);
	fscanf(f,"%d",&m);
	fscanf(f,"%d",&n);
	C = (float*) malloc (m * n * sizeof(float));
	for (int i = 0; i < m*n; ++i)
	{
		fscanf(f,"%f", C+i);
	}
	fclose(f);
}

void print(int m, int n){
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			printf("%3.2f ",*(C + i*n + j));
		}
		printf("\n");
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	readFile();
	print(m,n);
	dC = (float*) malloc (m * n * sizeof(float));
	for (int i = 0; i < m*n; ++i)
	{
		*(dC+i) = 0;
	}
	while( t<=T){
		FD(C, dC);
		for (int i = 0; i < m; ++i)
			for (int j = 0; j < n; ++j)
				*(C+i*n + j) = *(C+i*n + j) + dt*D*(*(dC+i*n + j));
		t = t + dt;
	}
	print(m,n);
	return 0;
}