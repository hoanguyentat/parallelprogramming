#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>

int n = 20;
int Ntime = 100;
float dt = 0.01;
float dx = 0.1;
float D = 0.1;
float *C, *dC, *cS, *dCs, dS, cU;

void print(){
	for (int i = 0; i < n; ++i)
		{
			printf("%3.2f ",*(C + i));
		}
	printf("\n");
}


void FD(float *cS, float *dCs, float dS, float cU, int mc){
	float c, l, r;
	for (int i = 0; i < mc; ++i)
	{
			c = *(cS + i);
			l = (i==0)? dS : *(cS+ i -1);
			r = (i == mc-1) ? cU: *(cS + i +1);
			*(dCs + i) = D*(l+r-2*c)/(dx*dx);
	}	
}
int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	dC = (float*) malloc (n * sizeof(float));
	C = (float*) malloc (n * sizeof(float));

	for (int i = 0; i < n; ++i){
		*(dC+i) = 0;
		*(C+i) = 25;	
	}

	MPI_Status stat;

	int mc = n/size;
	cS = (float*) malloc (mc * sizeof(float));
	dCs = (float*) malloc (mc * sizeof(float));

	MPI_Scatter(C, mc, MPI_FLOAT, cS, mc, MPI_FLOAT, 0, MPI_COMM_WORLD);
	for (float t = 0; t < Ntime; t+=dt){
		if (rank == 0){
			dS = 100;
			MPI_Send(cS + mc -1, 1, MPI_FLOAT,rank + 1, rank, MPI_COMM_WORLD);
		}
		else if (rank == size-1){
			cU = 25;
			MPI_Recv((void*) &dS, 1, MPI_FLOAT, rank-1, rank-1,MPI_COMM_WORLD, &stat);
		}
		else{
			MPI_Send(cS + mc - 1, 1, MPI_FLOAT, rank+1,rank, MPI_COMM_WORLD);
			MPI_Recv((void*) &dS, 1, MPI_FLOAT, rank-1, rank-1, MPI_COMM_WORLD,&stat);
		}

		if (rank == size -1){
			MPI_Send(cS, 1, MPI_FLOAT,rank - 1, rank, MPI_COMM_WORLD);
		} else if (rank == 0){
			MPI_Recv((void*) &cU, 1, MPI_FLOAT, rank+1, rank+1,MPI_COMM_WORLD, &stat);
		} else {
			MPI_Send(cS, 1, MPI_FLOAT, rank-1, rank,MPI_COMM_WORLD);
			MPI_Recv((void*) &cU, 1, MPI_FLOAT, rank+1, rank+1,MPI_COMM_WORLD, &stat);
		}
		FD(cS, dCs, dS, cU, mc);
		for (int i = 0; i < mc; ++i)
			*(cS+i) += dt * (*(dCs+i));		
	}
	MPI_Gather (cS, mc, MPI_FLOAT, C, mc, MPI_FLOAT, 0,MPI_COMM_WORLD);
	if (rank == 0){
		print();
	}
	return 0;
}