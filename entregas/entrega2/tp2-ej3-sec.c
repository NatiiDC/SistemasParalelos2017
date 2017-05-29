#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Time in seconds from some point in the past */
long double dwalltime();
void imprimirInferior(long long* A, long long N);
void imprimirSuperior(long long* A, long long N);

long long main (long long argc,char* argv[]){
	long double avg = 0;
	long long E, N, iter = 1;
	long long *A,*T;

  if (argc < 2) {
    printf("\n Falta un argumento: 2 ^ N");
    printf("\n Opcional: cantidad de iteraciones");
    printf("\n Compilar:\ngcc -o tp2-ej3-sec tp2-ej3-sec.c -lm -std=c99\n");

    return 0;
  }

	if (argc == 3) {
		iter = atoi(argv[2]);
	}
	long long power = atoi(argv[1]);
  N = pow(2, power);
	for (size_t i = 0; i < iter; i++) {
		E = N*(N+1)/2;
		A =(long long*)malloc(sizeof(long long)*E);
		T =(long long*)malloc(sizeof(long long)*E);

		for (long long i = 0; i < E; ++i) {
			A[i] = i+1;
			T[i] = 0;
		}
	  if (argc == 4) {
			if (atoi(argv[3]) == 1) {
			    printf("Matriz Inicial\n");
				imprimirInferior(A,N);
			}
		}

	  double timetick = dwalltime();
	  long long row = 0, column = 0, pos;
		for (long long i = 0; i < E; ++i) {
	    if (column > row) {
	      column = 0;
	      row++;
	    }
	    pos = (N * column) + row - ((column * (column+1)) / 2);
	    T[pos] = A[i];
	    column++;
		}

		long double tiempo = dwalltime() - timetick;
		printf("Tiempo en segundos %LF \n", tiempo );
		avg += tiempo;

		if (argc == 4) {
			if (atoi(argv[3]) == 1) {
			    printf("Matriz Resultante\n");
				imprimirSuperior(T,N);
			}
		}
		free(A);
		free(T);
	}
	printf("Tiempo promedio: %LF.\n", avg/ iter);
}


/****************************** PRINT ***********************************/

void imprimirInferior(long long* A, long long N) {
	long long actual = 0;
	for(long long i=0;i<N;i++){
		for(long long j=0;j<i+1;j++){
			printf("%d ", (long long) A[actual]);
			actual++;
		}
		for(long long j=i+1;j<N;j++){
			printf("0 ");
		}
		printf("\n");
	}
	printf("\n");
}

void imprimirSuperior(long long* A, long long N) {
	long long actual = 0;
	for(long long i=0;i<N;i++){
		for(long long j=0;j<i;j++){
			printf("0 ");
		}
		for(long long j=i;j<N;j++){
			printf("%d ", (long long) A[actual]);
			actual++;
		}
		printf("\n");
	}
	printf("\n");
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
