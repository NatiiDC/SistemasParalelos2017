#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

/* Time in seconds from some point in the past */
double dwalltime();
void* producto(void * num);
void imprimirMatrizFila(double* A);
int count_threads = 2;

double *A, *B, *RET;
int N;

struct My_threads {
  int id;
  pthread_t thread;
};

int main(int argc, char const *argv[]) {

  if (argc < 2) {
    printf("Debe ingresar el tamaño del arreglo (potencia de 2)\n");
    printf("Parametro opcional y (threads)\n");
    printf("./ejercicio1 20123 4\n");
    printf("Como compilar: gcc -pthread -o ejercicio1 ejercicio1.c -O3\n");
    return 1;
  }
  if (argc > 2 && atoi(argv[2]) == 4) {
    count_threads = 4;
  }

  double timetick;
  int iter = 10;
  N = atoi(argv[1]);
  A = (double*)malloc(sizeof(double) * N * N);
  B = (double*)malloc(sizeof(double) * N * N);
  RET = (double*)calloc(N * N, sizeof(double));

  for(int fila = 0; fila < N; fila++) {
    for(int col = 0; col < N; col++) {
      A[fila*N+col] = 1;
      B[fila+col*N] = 1;
    }
  }

  struct My_threads p_threads[count_threads];
  double promedio = 0;
  for (int a = 0; a < iter; a++) {

    timetick = dwalltime();

    for (int i = 0; i < count_threads; i++) {
      p_threads[i].id = i;
      pthread_create(&p_threads[i].thread, NULL, producto, &p_threads[i].id);
    }

    for (int j = 0; j < count_threads; j++) {
      pthread_join(p_threads[j].thread, NULL);
    }

    promedio += dwalltime() - timetick;
  }
  promedio = promedio / iter;
  printf("Tiempo promedio para %d iteracion/es: %f \n", iter, promedio);

  // printf("Tiempo en segundos: %f \n", dwalltime() - timetick);
  // printf("\nMatriz: ");
  // imprimirMatrizFila(A);
  // printf("\nResultante: ");
  // imprimirMatrizFila(RET);

  return 0;
}

// ----------------------- functions -----------------------
void* producto(void* num){
  int desplazamiento = *((int *)num);

  int desde = (int)(N / count_threads) * desplazamiento;
  int hasta = (int)(N / count_threads) * (desplazamiento + 1);
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


/*

gcc -pthread -o ejercicio1 ejercicio1.c
$ ./ejercicio1 512
Tiempo en segundos: 0.365698
$ ./ejercicio1 512 4
Tiempo en segundos: 0.378874
$ ./ejercicio1 1024
Tiempo en segundos: 2.806816
$ ./ejercicio1 1024 4
Tiempo en segundos: 2.782497
$ ./ejercicio1 2048
Tiempo en segundos: 35.509453
$ ./ejercicio1 2048 4
Tiempo en segundos: 29.553997


gcc -pthread -o ejercicio1 ejercicio1.c -O3
$ ./ejercicio1 1024
Tiempo promedio para 10 iteracion/es: 0.820173
$ ./ejercicio1 1024 4
Tiempo promedio para 10 iteracion/es: 0.546501
$ ./ejercicio1 2048
Tiempo promedio para 10 iteracion/es: 6.877718
$ ./ejercicio1 2048 4
Tiempo promedio para 10 iteracion/es: 5.981148


Aula PC PostGrado Maquina 13

gcc -pthread -o ejercicio1 ejercicio1.c -O3

./ejercicio1 1024
Tiempo promedio para 10 iteracion/es: 0.595256 
./ejercicio1 1024 4
Tiempo promedio para 10 iteracion/es: 0.309369 
./ejercicio1 2048 
Tiempo promedio para 10 iteracion/es: 4.846422 
./ejercicio1 2048 4
Tiempo promedio para 10 iteracion/es: 3.132956 

gcc -pthread -o ejercicio1 ejercicio1.c

./ejercicio1 1024
Tiempo promedio para 5 iteracion/es: 1.966145 
./ejercicio1 1024 4
Tiempo promedio para 5 iteracion/es: 1.026348 
./ejercicio1 2048
Tiempo promedio para 5 iteracion/es: 15.750792 
./ejercicio1 2048 4
Tiempo promedio para 5 iteracion/es: 8.495388

*/
