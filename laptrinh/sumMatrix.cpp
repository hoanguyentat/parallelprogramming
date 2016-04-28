#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>
#define n 10
int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);		//start mpi
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // get current process id
	MPI_Comm_size(MPI_COMM_WORLD,&size);	//number of processes
	MPI_Status stat;
	int* a0;
	int* b0;
	int* c0;
	int *c1, *a1, *b1;
	int num = n/size;
	c0 = (int*) malloc (n * sizeof(int));
	a0 = (int*) malloc (n * sizeof(int));
	b0 = (int*) malloc (n * sizeof(int));

	a1 = (int*) malloc (num * sizeof(int));
	b1 = (int*) malloc (num * sizeof(int));
	c1 = (int*) malloc (num * sizeof(int));
	for (int i = 0; i < num; ++i)
	{
		c1[i] = 0;
	}
	if (rank == 0){
		
		for (int i = 0; i < n; ++i)
		{
			a0[i] = rand()%10;
			b0[i] = rand()%10;
		}
	}
	if (rank == 0)
	{
		for (int i = 0; i < 10; ++i)
		{
			printf("%d ",a0[i]);
		}
	}
	MPI_Scatter(a0, num, MPI_INT, a1, num, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b0, num, MPI_INT, b1, num, MPI_INT, 0, MPI_COMM_WORLD);
	
	for (int i = 0; i < num; ++i){
		c1[i] = a1[i] + b1[i];
	}
	if (rank == 0)
	{
		printf("\n");
		for (int i = 0; i < num; ++i)
		{
			printf("%d ",a1[i]);
		}
		printf("\n");
		for (int i = 0; i < num; ++i)
		{
			printf("%d ",b1[i]);
		}
		printf("\n");
		for (int i = 0; i < num; ++i)
		{
			printf("%d ",c1[i]);
		}
		printf("\n");
	}

MPI_Gather(c1, num, MPI_INT, 
				c0, num, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < n; ++i)
	{
		printf("Hello %d\n",c0[i]);
	}
	// printf("Hello\n");
	return 0;
}