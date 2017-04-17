#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define COUNT_THREADS 2

/* Time in seconds from some point in the past */
double dwalltime();
void* producto(void * num);
void imprimirMatrizFila(double* A);

double *A, *B, *RET;
int N;

int main(int argc, char const *argv[]) {

  if (argc < 2) {
      printf("Debe ingresar el tamaño del arreglo (potencia de 2)\ny");
      printf("./ejercicip1 N\n");
      return 1;
  }

  double timetick;

  N = atoi(argv[1]);
  A = (double*)malloc(sizeof(double) * N * N);
  B = (double*)malloc(sizeof(double) * N * N);
  RET = (double*)calloc(N * N, sizeof(double));

  timetick = dwalltime();
  for(int fila = 0; fila < N; fila++) {
    for(int col = 0; col < N; col++) {
      A[fila*N+col] = 1;
      B[fila+col*N] = 1;
    }
  }

  pthread_t p_threads[COUNT_THREADS];

  int valor1=0, valor2=1;
  pthread_create(&p_threads[valor1], NULL, producto, &valor1);
  pthread_create(&p_threads[valor2], NULL, producto, &valor2);

  for (int j = 0; j < COUNT_THREADS; j++) {
    pthread_join(p_threads[j], NULL);
  }

  printf("Tiempo en segundos: %f \n", dwalltime() - timetick);
  // printf("\nMatriz: ");
  // imprimirMatrizFila(A);
  //
  // printf("\nResultante: ");
  // imprimirMatrizFila(RET);

  return 0;
}

// ----------------------- functions -----------------------
void* producto(void* num){
  int desplazamiento = *((int *)num);

  int desde = (int)(N / COUNT_THREADS) * desplazamiento;
  int hasta = (int)(N / COUNT_THREADS) * (desplazamiento + 1);
  // printf("\n%d",*(int *)num);
  // printf("\nid:%d \ndesde: %d; hasta: %d\n", desplazamiento, desde, hasta );
	double aux;

	for(int i = desde; i < hasta; i++){
    for(int j = 0; j < N; j++){
      aux = 0;
      for(int k = 0; k < N; k++){
        aux = aux + A[i*N + k] * B[k + j*N];
      }
      RET[i*N + j] = aux;
   }
  }
  pthread_exit(0);
}

void imprimirMatrizFila(double* A) {
  printf("\n");
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      printf("%d ", (int) A[i*N+j]);
    }
    printf("\n");
  }
  printf("\n");
}

/*****************************************************************/

#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
