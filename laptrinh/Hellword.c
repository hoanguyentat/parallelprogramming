#include <stdio.h>
#include <omp.h>
int main(){
	#pragma omp parallel {
	printf("Hello world thread %d, nthreads %d\n",
	omp_get_thread_num(), //tra ve luong dang chay
	omp_get_num_threads()); //tra ve so luong duoc su dung
	}
	return 0;
}