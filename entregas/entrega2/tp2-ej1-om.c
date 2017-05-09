#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

/* Time in seconds from some point in the past */
long double dwalltime();

void productoEscalar(double x, double* A, int N);
double promedioMatriz(double *M, int n);
void producto(double *ret, double *pri, double *seg, int n);
void suma(double *A, double *B, int n);
void imprimirMatrizColumna(double* A, int N);
void imprimirMatrizFila(double* A, int N);

double *A, *B, *C ,*D ,*E ,*F, *AB, *ABC, *DE, *DEF;
double promB = 0, promD = 0;
int numThreads = 2;
long double timetick;
int N, iter = 2;

int main( int argc, char* argv[] ) {

	//Controla los argumentos al programa
	if (argc < 2) {
		printf("\n Falta un argumento: N dimension de la matriz \n");
		printf("\n 2do argumento opcional: cantidad de hilos \n");
		printf("\n Compilar:\ngcc -fopenmp -o tp2-ej1-om tp2-ej1-om.c -lm\n");

		return 0;
	}

  N = atoi(argv[1]);
	if (argc >= 3 && atoi(argv[2]) == 4) {
    numThreads = 4;
	}
  omp_set_num_threads(numThreads);

	if (argc == 4) {
		iter = atoi(argv[3]);
	}

  printf("CANTIDAD DE THREADS: %d.\n", numThreads);

  //Aloca memoria para las matrices
  A = (double*)malloc(sizeof(double)*N*N);
  B = (double*)malloc(sizeof(double)*N*N);
  C = (double*)malloc(sizeof(double)*N*N);
  D = (double*)malloc(sizeof(double)*N*N);
  E = (double*)malloc(sizeof(double)*N*N);
  F = (double*)malloc(sizeof(double)*N*N);
  AB = (double*)calloc(N*N, sizeof(double));
  ABC = (double*)calloc(N*N, sizeof(double));
  DE = (double*)calloc(N*N, sizeof(double));
  DEF = (double*)calloc(N*N, sizeof(double));

  //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      A[i*N+j] = 1;
      B[i+j*N] = 1;
      C[i+j*N] = 1;
      D[i*N+j] = 1;
      E[i+j*N] = 1;
   		F[i+j*N] = 1;
    }
  }

	long double promedio = 0;
	for (int a = 0; a < iter; a++) {

		//arrancan las operaciones y el contador
	  timetick = dwalltime();

	  promB = promedioMatriz(B, N);
	  promD = promedioMatriz(D, N);

	  productoEscalar(promD, A, N);
	  productoEscalar(promB, D, N);

		producto(AB, A, B, N);
	  producto(ABC, AB, C, N);
	  producto(DE, D, E, N);
	  producto(DEF, DE, F, N);

		suma(ABC, DEF, N);
		promedio += (dwalltime() - timetick);
	}
	promedio = promedio / iter;
	printf("Tiempo promedio para %d iteracion/es: %LF \n", iter, promedio);

  free(A);
  free(B);
  free(C);
  free(D);
  free(E);
  free(F);
  free(AB);
  free(ABC);
  free(DE);
  free(DEF);

}

void productoEscalar(double x, double* A, int N) {
  #pragma omp parallel for
  for(int i=0; i < (N*N); i++){
    A[i] *= x;
  }
}

double promedioMatriz(double *M, int n){
	double total = 0.0;
	int i;

  #pragma omp parallel for reduction(+: total)
	for (i = 0; i < n*n; i++){
		total += M[i];
	}

  total = total / (n*n);

	return total;
}

void producto(double *ret, double *pri, double *seg, int N){
	int i,j,k;
	double aux;

  #pragma omp parallel for private(aux, j, k)
	for(i = 0; i < N; i++){
    for(j = 0; j < N; j++){
      aux = 0;
      for(k = 0; k < N; k++){
        aux = aux + pri[i*N + k] * seg[k + j*N];
      }
      ret[i*N + j] = aux;
   }
  }
}

void suma(double *A, double *B, int N) {
  #pragma omp parallel for
  for(int i = 0; i < (N*N); i++){
    A[i] += B[i];
  }
}

void imprimirMatrizFila(double* A, int N) {
  printf("\n");
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      printf("%d ", (int) A[i*N+j]);
    }
    printf("\n");
  }
  printf("\n");
}

void imprimirMatrizColumna(double* A, int N) {
  printf("\n");
  for(int i=0;i<N;i++){
    	for(int j=0;j<N;j++){
      printf("%d ", (int) A[i+j*N]);
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
