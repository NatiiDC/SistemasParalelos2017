#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

/* Time in seconds from some point in the past */
long double dwalltime();

int main (int argc,char* argv[]){
	int E, N, count_threads = 2, iter = 1;
	int *A,*T;

  if (argc < 2) {
    printf("\n Falta un argumento: N dimension de la matriz \n");
    printf("\n 2do argumento opcional: cantidad de hilos \n");
    printf("\n Compilar:\ngcc -pthread -o tp2-ej3 tp2-ej3.c -lm\n");

    return 0;
  }

	if (argc >= 3 && atoi(argv[2]) == 4) {
		count_threads = 4;
	}
	if (argc == 4) {
		iter = atoi(argv[3]);
	}

  N = atoi(argv[1]);
  double timetick = dwalltime();
	omp_set_num_threads(count_threads);

	E = N*(N+1)/2;
	A =(int*)malloc(sizeof(int)*E);
	T =(int*)malloc(sizeof(int)*E);

	for (int i = 0; i < E; ++i) {
		A[i] = i+1;
		T[i] = 0;
	}

  #pragma omp parallel for
	for (int i = 0; i < E; ++i) {
    int pos = i;
    int row = 0;
    while (pos >= 0) {
    	pos = pos - (row+1);
    	row++;
    }
    row--;
    int col = i - (row*(row+1)/2);
    int pos = (N * col) + row - ((col * (col+1)) / 2);
    T[pos] = A[i];
	}


	double tiempo = dwalltime() - timetick;

	printf("Tiempo en segundos %f \n", tiempo );

}


/*****************************************************************/

#include <sys/time.h>

long double dwalltime()
{
	long double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
