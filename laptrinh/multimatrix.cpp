#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#define A 4
#define	B 4
#define C 5

int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);		
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status stat;
	if (rank == 0){
		int *a0;
		int *b0;
		int *c0;
		//Cap phat bo nho dong
		a0 = (int*) malloc(A * B * sizeof(int));
		b0= (int*) malloc(B * C * sizeof(int));
		c0 = (int*) malloc(A * C * sizeof(int));

		//khoi tao gia tri
		for (int i = 0; i < A*B; ++i){
			a0[i] = rand() % 10;
			// printf("a[%d]: %d\n",i, a0[i] );
		}

		for (int i = 0; i < C*B; ++i){
			b0[i] = rand() % 5;
		}

		for (int i = 0; i < A * B; ++i)
		{
			if (i % B == 0) printf("\n");
			printf("%d ", a0[i]);
		}

		printf("\n");
		for (int i = 0; i < B * C; ++i)
		{
			if (i % C == 0) printf("\n");
			printf("%d ", b0[i]);
		}

		printf("\n");

		printf("\n");
		int numCol = A/size;
		int colMod = A - numCol*size;

		for (int i = 1; i < size; ++i)
		{
			MPI_Send((void*) &numCol, 1, MPI_INT, i, 10*i+1, MPI_COMM_WORLD);
			// Gui mot phan cua a0
			MPI_Send(a0 + colMod*B + i*numCol*B, numCol*B, MPI_INT, i, i, MPI_COMM_WORLD);
			// gui ca b0
			MPI_Send(b0, B*C, MPI_INT, i, 10*i, MPI_COMM_WORLD);
			//Nhan tung phan cua c0
			MPI_Recv(c0 + colMod*C + i*numCol*C , numCol*C, MPI_INT, i, 100*i, MPI_COMM_WORLD, &stat);
		}
		//Nhan ma tran voi so hang la so du + so hang cua mot rank
		for (int i = 0; i < numCol + colMod; ++i)
		{
			for (int j = 0; j < C; ++j)
			{
				c0[i*C + j] =0;
				for (int k = 0; k < B; ++k)
				{
					c0[i*C + j] += a0[i*B + k] * b0[k*C + j];
				}
			}
		}

		// In danh sach ma tran
		for (int i = 0; i < A * C; ++i)
		{
			if (i % C == 0) printf("\n");
			printf("%d ", c0[i]);
		}

		printf("\n");


	}
	else {
		int *a1;
		int *b1;
		int *c1;
		int nC;
		MPI_Recv((void*) &nC, 1, MPI_INT, 0, 10*rank+1, MPI_COMM_WORLD, &stat);

		a1 = (int*) malloc(nC * B * sizeof(int));
		b1 = (int*) malloc(B * C * sizeof(int));
		c1 = (int*) malloc(nC * C * sizeof(int));

		//Nhan gia tri gui tu rank 0
		MPI_Recv(a1, nC * B, MPI_INT, 0, rank, MPI_COMM_WORLD, &stat);
		MPI_Recv(b1, B*C, MPI_INT, 0, 10*rank, MPI_COMM_WORLD, &stat);

		//Tinh toan gia tri cua 1 rank
		for (int i = 0; i < nC; ++i)
		{
			for (int j = 0; j < C; ++j)
			{
				c1[i*C + j] = 0;
				for (int k = 0; k < B; ++k)
				{
					c1[i*C + j] += a1[i*B + k] * b1[k*C + j];
				}
			}
		}

		//Gui ket qua ve rank 0
		MPI_Send(c1, nC*C, MPI_INT, 0, 100*rank, MPI_COMM_WORLD);
	}
	return 0;
}