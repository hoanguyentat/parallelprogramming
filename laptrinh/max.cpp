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
		int recvMax;
		int A[10] = {0, 1, 2, 3, 4, 5 ,6, 7, 8, 9};
		MPI_Send(A, 10, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Recv((void*) &recvMax, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &stat);
		printf("Gia tri max cua B: %d\n", recvMax);
	}
	
	else if (rank == 1)
	{
		int max = 0;
		int B[10];
		MPI_Recv(B, 10, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
		for (int i = 0; i < 10; ++i)
		{
			if (max < B[i]) max = B[i];
			printf("%d ",B[i]);
		}
		printf("\n");
		MPI_Send((void*) &max, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}