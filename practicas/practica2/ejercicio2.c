#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

/* Time in seconds from some point in the past */
double dwalltime();
void* buscar(void * num);
void imprimirMatrizFila(double* A);

typedef struct My_threads {
  int id;
  pthread_t thread;
  int result;
} my_thread;

int count_threads = 2, N, find;
int *vector;

struct My_threads* p_threads;

int main(int argc, char const *argv[]) {

  if (argc < 3) {
    printf("./ejercicio2 tamañoDelVector numero [4]\n");
    printf("Debes ingresar el tamaño del vecto y numero a encontrar (0 <= x <= 100)\n");
    printf("Parametro opcional 2 o 4 threads\n");
    printf("Como compilar: gcc -pthread -o ejercicio2 ejercicio2.c -O3\n");
    return 1;
  }
  if (argc > 3 && atoi(argv[3]) == 4) {
    count_threads = 4;
  }

  double timetick;
  int iter = 50;
  N = atoi(argv[1]);
  find = atoi(argv[2]);

  printf("CANTIDAD DE THREADS: %d.\n", count_threads);

  vector = (int*)malloc(sizeof(int) * N);
  p_threads = malloc(sizeof(struct My_threads) * count_threads);

  for (int it = 0; it < N; it++) {
    vector[it] = (it*2)%100;
  }

  double promedio = 0;
  int tot;

  for (int a = 0; a < iter; a++) {

    timetick = dwalltime();
    tot = 0;

    for (int i = 0; i < count_threads; i++) {
      p_threads[i].id = i;
      pthread_create(&p_threads[i].thread, NULL, buscar, &p_threads[i].id);
    }

    for (int j = 0; j < count_threads; j++) {
      pthread_join(p_threads[j].thread, NULL);
    }

    for (int k = 0; k < count_threads; k++) {
      tot += p_threads[k].result;
    }

    promedio += dwalltime() - timetick;
  }
  promedio = promedio / iter;
  printf("%d elementos encontrados.\n", tot);
  printf("Tiempo promedio para %d iteracion/es: %f \n", iter, promedio);

  return 0;
}

// ----------------------- functions -----------------------
void* buscar(void* num){
  int desplazamiento = *((int *)num);

  int desde = (int)(N / count_threads) * desplazamiento;
  int hasta = (int)(N / count_threads) * (desplazamiento + 1);
	int ocur = 0;
  // printf("\n%d",*(int *)num);
  // printf("\nid:%d \ndesde: %d; hasta: %d\n", desplazamiento, desde, hasta );
	for(int i = desde; i < hasta; i++){
    if (vector[i] == find) {
      ocur++;
    }
  }
  p_threads[desplazamiento].result = ocur;

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
CONCLUSION DE REITERADAS EJECUCIONES:

Sin optimizar, a partir de 25.000 elementos aprox. es más rapido con 4 threads.
Optimizado, a partir de 30.000.000 de elementos aprox. es un poco más rápido con 4 threads.
*/
