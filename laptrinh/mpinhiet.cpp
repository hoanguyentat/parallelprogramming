#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <malloc.h>

float t = 0; 
float T = 2;
float dt = 0.01;
float dx = 0.1;
float D = 0.1;
float *C, *dCs, *Cu, *Cd, *Cs;
int m, n;

void readFile(){
	FILE *f = fopen("file.txt","rt");
	if (f == NULL)
	{
		printf("Khong mo duoc file\n");
		exit(1);
	}
	rewind(f);
	int doc = fscanf(f,"%d %d",&m,&n);
	C = (float*) malloc (m * n * sizeof(float));
	for (int i = 0; i < m*n; ++i)
	{
		doc = fscanf(f,"%f",C + i);
	}
	fclose(f);
}

void FD(float *Cs, float *Cu, float *Cd, float *dCs ,int ms) {
	int i, j;
	float c,u,d,l,r;
	for ( i = 0 ; i < ms ; i++ )
		for ( j = 0 ; j < n ; j++ ){
			c = *(Cs+i*n+j);
			u = (i==0) ? *(Cu+j) : *(Cs+(i-1)*n+j);
			d = (i==ms-1) ? *(Cd+j) : *(Cs+(i+1)*n+j);
			l = (j==0) ? *(Cs+i*n+j) : *(Cs+i*n+j-1);
			r = (j==n-1) ? *(Cs+i*n+j) : *(Cs+i*n+j+1);
			*(dCs+i*n+j) = (1
				/(dx*dx))*(u+d+l+r-4*c);
		}
}

int main(int argc, char *argv[])
{
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Status stat;

	readFile();
	if (rank == 0){
		printf("Nhiet do ban dau:\n");
		for (int i = 0; i < m; ++i){
			for (int j = 0; j < n; ++j){
				printf("%3.2f ",*(C + i*n + j));
			}
			printf("\n");
		}
	}
	printf("\n");

	// printf("%d %d\n",m,n);
	int mc = m/size;
	Cu = (float*) malloc (n * sizeof(float));
	Cd = (float*) malloc (n * sizeof(float));
	Cs = (float*) malloc (mc*n * sizeof(float));
	dCs = (float*) malloc (m * n * sizeof(float));
	
	MPI_Scatter (C, mc*n, MPI_FLOAT,Cs, mc*n, MPI_FLOAT,0,MPI_COMM_WORLD);


	while (t<=T){
		
		if (rank==0){
			for (int j = 0; j< n; j++) *(Cu+j) = *(Cs+0*n+j);
			MPI_Send (Cs+(mc-1)*n, n, MPI_FLOAT, rank+1, rank, MPI_COMM_WORLD);
		} 
		else if (rank==size-1) {
			MPI_Recv (Cu, n, MPI_FLOAT, rank-1, rank-1, MPI_COMM_WORLD,&stat);
			} else {
				MPI_Send(Cs+(mc-1)*n, n, MPI_FLOAT, rank+1, rank ,MPI_COMM_WORLD);
				MPI_Recv(Cu, n, MPI_FLOAT, rank-1, rank-1, MPI_COMM_WORLD,&stat);
		}		

		if (rank==size-1){
			for (int j=0; j<n; j++) *(Cd+j) = *(Cs+(mc-1)*n+j);
			MPI_Send(Cs, n, MPI_FLOAT, rank-1, rank, MPI_COMM_WORLD);
		} else if (rank==0) {
			MPI_Recv(Cd, n, MPI_FLOAT, rank+1, rank+1, MPI_COMM_WORLD,&stat);
			} else {
				MPI_Send(Cs, n, MPI_FLOAT, rank-1, rank, MPI_COMM_WORLD);
				MPI_Recv(Cd, n, MPI_FLOAT, rank+1, rank+1, MPI_COMM_WORLD, &stat);
		}
		FD(Cs,Cu,Cd,dCs,mc);
		for (int i = 0; i < mc; ++i)
			for (int j = 0; j < n; ++j)
				*(Cs+i*n + j) = *(Cs+i*n + j) + dt*D*(*(dCs+i*n + j));
		t = t + dt;
	}
	MPI_Gather (Cs, mc*n, MPI_FLOAT, C, mc*n, MPI_FLOAT, 0,MPI_COMM_WORLD);
	
	if (rank == 0){
		printf("Nhiet do luc sau:\n");
		for (int i = 0; i < m; ++i){
			for (int j = 0; j < n; ++j){
				printf("%3.2f ",*(C + i*n + j));
			}
			printf("\n");
		}
	}
	return 0;
}