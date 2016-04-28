#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define A 3
#define B 4
#define C 5

//Hien thi ma tran
void displayMatrix(int** Matrix, int m, int n)
{
	for (int i = 0; i < m; ++i)
	{
		for (int k = 0; k < n; ++k)
		{
			printf("%d ",Matrix[i][k]);
		}
		printf("\n");
	}
	printf("\n");
}
int main(int argc, char const *argv[])
{
	int **a;
	int **b;
	int **c;
	int **d;
	int **e;
	
//Cap phat bo nho dong
	a = (int**) malloc(A * sizeof(int*));
	b = (int**) malloc(B * sizeof(int*));
	c = (int**) malloc(A * sizeof(int*));
	d = (int**) malloc(A * sizeof(int*));
	e = (int**) malloc(A * sizeof(int*));

	for (int i = 0; i < A; ++i)
	{
		a[i] = (int*) malloc(B * sizeof(int));
		c[i] = (int*) malloc(C * sizeof(int));
		d[i] = (int*) malloc(C * sizeof(int));
		e[i] = (int*) malloc(C * sizeof(int));
	}
	for (int i = 0; i < B; ++i)
	{
		b[i] = (int*) malloc(C * sizeof(int));
	}
//khoi tao gia tri
	for (int i = 0; i < A; ++i)
	{
		for (int j = 0; j < B; ++j)
		{
			a[i][j] = (i == j) ? 1 : 0;
		}
	}

	for (int i = 0; i < B; ++i)
	{
		for (int j = 0; j < C; ++j)
		{
			b[i][j] = rand() % 100;
		}
	}

	for (int i = 0; i < A; ++i)
	{
		for (int j = 0; j < C; ++j)
		{
			c[i][j] = 1;
		}
	}

	for (int i = 0; i < A; ++i)
	{
		for (int j = 0; j < C; ++j)
		{
			d[i][j] = 1;
		}
	}
	for (int i = 0; i < A; ++i)
	{
		for (int j = 0; j < C; ++j)
		{
			e[i][j] = 1;
		}
	}
//nhan ma tran
	for (int i = 0; i < A; ++i)
	{
		for (int k = 0; k < C; ++k)
		{
			int sum = 0;
			for (int j = 0; j < B; ++j)
			{
				sum+= a[i][j] * b[j][k];
				// printf("%d\n", sum);
			}
			c[i][k] = sum;
		}
	}
	displayMatrix(c, A, C);

//Da luong: so luong so o cua ma tran kq
	int* sum1 = (int*) malloc(A * C * sizeof(int));
		for (int i = 0; i < A*C; ++i)
		{
			sum1[i] = 0;
		}
	omp_set_num_threads(A*C);
	#pragma omp parallel
	{
		int id, m, p;
		id = omp_get_thread_num();
		m = id/C;
		p = id%C;

		for (int i = 0; i < B; ++i)
		{
			sum1[id]+= a[m][i]*b[i][p];
		}
		d[m][p] = sum1[id];
	}

	displayMatrix(d,A,C);

//Da luong: so luong bang so hang ma tran ket qua
	// int* sum2 = (int*) malloc(A * C * sizeof(int));
	// for (int i = 0; i < A*C; ++i)
	// {
	// 	sum2[i] = 0;
	// }
	// omp_set_num_threads(A);
	// #pragma omp parallel
	// {
	// 	int id;
	// 	id = omp_get_thread_num();
	// 	for (int i = 0; i < C; ++i)
	// 	{
	// 		for (int j = 0; j < B; ++j)
	// 		{
	// 			sum2[id*C+i] += a[id][j]*b[j][i];
	// 			// printf("%d %d ", a[id][j], b[j][i]);
	// 			// printf("%d\n\n",sum2[id]);
	// 		}
	// 		// printf("%d\n",sum2 );
	// 		e[id][i] = sum2[id*C+i];
	// 	}
	// }

	// displayMatrix(e,A,C);
//da luong: so luong bang voi so cot cua ma tran ket qua
	int* sum3 = (int*) malloc(A * C * B * sizeof(int));
	for (int i = 0; i < A*C*B; ++i)
	{
		sum3[i] = 0;
	}
	omp_set_num_threads(B);
	#pragma omp parallel
	{
		int id;
		id = omp_get_thread_num();
		for (int i = 0; i < A; ++i)
		{
			for (int j = 0; j < C; ++j)
			{
				sum3[j*A*C+B*i+id] = a[i][id] * b[id][j];
			}
		}
	}
	for (int i = 0; i < A; ++i)
	{
		for (int j = 0; j < C; ++j)
		{
			for (int k = 0; k < B; ++k)
			{
				e[i][j] +=  sum3[i*B+k];
			}
		}
	}
	displayMatrix(e,A,C);
	return 0;
}