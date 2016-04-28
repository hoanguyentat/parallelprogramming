#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include "Timer.cpp"

int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);		//start mpi
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // get current process id
	MPI_Comm_size(MPI_COMM_WORLD,&size);	//number of processes
	MPI_Status stat;
	if (rank == 0){
		int a0[20];
		for (int i = 0; i < 20; ++i)
		{
			a0[i] = i;
		}
		int b0[20];
		for (int i = 0; i < 20; ++i)
		{
			b0[i] = 20-i;
		}
		int c0[20];
		MPI_Send(a0, 20, MPI_INT, 1, 10, MPI_COMM_WORLD);
		MPI_Send(b0, 20, MPI_INT, 1, 11, MPI_COMM_WORLD);
		MPI_Recv(c0, 20, MPI_INT, 1, 12, MPI_COMM_WORLD, &stat);
		for (int i = 0; i < 20; ++i)
		{
			printf("%d ",c0[i]);
		}
		printf("\n");
	}
	else if (rank == 1){
		int a1[20];
		int b1[20];
		int c1[20];
		MPI_Recv(a1, 20, MPI_INT, 0, 10, MPI_COMM_WORLD, &stat);
		MPI_Recv(b1, 20, MPI_INT, 0, 11, MPI_COMM_WORLD, &stat);
		for (int i = 0; i < 20; ++i)
		{
			c1[i] = a1[i] + b1[i];
		}
		MPI_Send(c1, 20, MPI_INT, 0, 12, MPI_COMM_WORLD);
	}
	// printf("Hello\n");
	return 0;
}