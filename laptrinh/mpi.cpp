#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);		//start mpi
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // get current process id
	MPI_Comm_size(MPI_COMM_WORLD,&size);	//number of processes
	printf("%d of %d\n",rank, size);
	MPI_Finalize();
	return 0;
}
