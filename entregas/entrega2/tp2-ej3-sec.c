#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

/* Time in seconds from some point in the past */
long double dwalltime();
void imprimirInferior(int* A, int N);
void imprimirSuperior(int* A, int N);

int main (int argc,char* argv[]){
	int E, N, iter = 1;
	int *A,*T;

  if (argc < 2) {
    printf("\n Falta un argumento: N dimension de la matriz");
    printf("\n Opcional: cantidad de iteraciones");
    printf("\n Compilar:\ngcc -pthread -o tp2-ej3-sec tp2-ej3-sec.c -lm\n");

    return 0;
  }

	if (argc == 3) {
		iter = atoi(argv[2]);
	}

  N = atoi(argv[1]);

	E = N*(N+1)/2;
	A =(int*)malloc(sizeof(int)*E);
	T =(int*)malloc(sizeof(int)*E);

	for (int i = 0; i < E; ++i) {
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
  int row = 0, column = 0, pos;
	for (int i = 0; i < E; ++i) {
    if (column > row) {
      column = 0;
      row++;
    }
    pos = (N * column) + row - ((column * (column+1)) / 2);
    T[pos] = A[i];
    column++;
	}


	double tiempo = dwalltime() - timetick;
	printf("Tiempo en segundos %f \n", tiempo );

	if (argc == 4) {
		if (atoi(argv[3]) == 1) {
		    printf("Matriz Resultante\n");
			imprimirSuperior(T,N);
		}
	}
}


/****************************** PRINT ***********************************/

void imprimirInferior(int* A, int N) {
	int actual = 0;
	for(int i=0;i<N;i++){
		for(int j=0;j<i+1;j++){
			printf("%d ", (int) A[actual]);
			actual++;
		}
		for(int j=i+1;j<N;j++){
			printf("0 ");
		}
		printf("\n");
	}
	printf("\n");
}

void imprimirSuperior(int* A, int N) {
	int actual = 0;
	for(int i=0;i<N;i++){
		for(int j=0;j<i;j++){
			printf("0 ");
		}
		for(int j=i;j<N;j++){
			printf("%d ", (int) A[actual]);
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
