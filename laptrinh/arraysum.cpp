#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);		//start mpi
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // get current process id
	MPI_Comm_size(MPI_COMM_WORLD,&size);	//number of processes
	MPI_Status stat;
	if (rank == 0)
	{
		int a0[20];
		int b0[20];
		int c0[20];
		
	}
	return 0;
}