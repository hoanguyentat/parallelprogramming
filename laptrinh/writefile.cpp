#include <stdio.h>
#include <stdlib.h>

void writeFile(int m, int n){
	FILE *f = fopen("file.txt","w");
	if (f == NULL){
		printf("Khong mo duoc file\n");
		exit(1);
	}
	fprintf(f, "%d %d\n",m,n);
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			fprintf(f,"%.2f ",25.0);
		}
		fprintf(f,"\n");
	}
	fclose(f);
}

int main(int argc, char const *argv[])
{
	writeFile(20, 20);
	return 0;
}