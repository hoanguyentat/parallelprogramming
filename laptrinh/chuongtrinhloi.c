#include <stdio.h>
#include <omp.h>
int main(){
	omp_set_num_threads(100);
	#pragma omp parallel
	{
		int id, x;
		id = omp_get_thread_num();
		x = 10*id;
		printf("\n");
		printf("Hello from thread %d, x = %d\n",id, x);
		printf("\n");
	}
	return 0;
}
