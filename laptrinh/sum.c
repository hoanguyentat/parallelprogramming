#include <stdio.h>
#include <omp.h>
#define N 144
int main(void){
	int count = 0;
	int i = 0;
	int sum[10] = {0};
	int threadsize = N/10;
	int mode = N%10;
	int a[N], sum1 = 0, sum2 = 0;
	for (i = 0; i< N; i++){
		a[i] = i;
	}
	while(count < N){
		sum1+=a[count];
		count++;
	}

	omp_set_num_threads(10);
	#pragma omp parallel
	{
		int  id, li, start, stop;
		id = omp_get_thread_num();
		if (id < 9)
		{
			start = id*threadsize;
			stop = start + threadsize;
			for(li = start; li < stop; li++){
				sum[id]+= a[li];
			}
		}
		else {
			start = id*threadsize;
			for(li = start; li < N; li++){
				sum[id]+= a[li];
			}
		}
	}
	for ( i = 0; i < 10; ++i)
			{
				sum2 += sum[i];
			}
	printf("%d\n%d\n",sum1,sum2 );
	return 0;
}