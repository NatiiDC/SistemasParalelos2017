#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

/* Time in seconds from some point in the past */
double dwalltime();
void* buscar(void * num);

typedef struct My_threads {
  int id;
  pthread_t thread;
  long total_parcial;
} my_thread;

int count_threads = 2, find;
long *vector;
long long N;

struct My_threads* p_threads;

int main(int argc, char const *argv[]) {

  if (argc < 2) {
    printf("./ejercicio5 tamañoDelVector [4]\n");
    printf("Debes ingresar el tamaño del vecto \n");
    printf("Parametro opcional 2 o 4 threads\n");
    printf("Como compilar: gcc -pthread -o ejercicio5 ejercicio5.c -O3\n");
    return 1;
  }
  if (argc > 2 && atoi(argv[2]) == 4) {
    count_threads = 4;
  }

  double timetick;
  int iter = 10;
  N = atoll(argv[1]);

  printf("CANTIDAD DE THREADS: %d.\n", count_threads);

  vector = (long*)malloc(sizeof(long long) * N);
  p_threads = malloc(sizeof(struct My_threads) * count_threads);

  for (int it = 0; it < N; it++) {
    vector[it] = ((it*2)+(it*7))%100;
  }

  double promedio = 0;
  long tot;

  for (int a = 0; a < iter; a++) {

    tot = 0;
    timetick = dwalltime();

    for (int i = 0; i < count_threads; i++) {
      p_threads[i].id = i;
      pthread_create(&p_threads[i].thread, NULL, buscar, &p_threads[i].id);
    }

    for (int j = 0; j < count_threads; j++) {
      pthread_join(p_threads[j].thread, NULL);
    }

    for (int k = 0; k < count_threads; k++) {
      tot += p_threads[k].total_parcial;
    }
    tot = tot / N;
    promedio += dwalltime() - timetick;
    // printf("Valor promedio del vector: %ld.\n", tot);
  }
  promedio = promedio / iter;

  printf("Tiempo promedio para %d iteracion/es: %f \n", iter, promedio);

  return 0;
}

// ----------------------- functions -----------------------
void* buscar(void* num){
  int desplazamiento = *((int *)num);

  int desde = (int)(N / count_threads) * desplazamiento;
  int hasta = (int)(N / count_threads) * (desplazamiento + 1);
  long totalParcial = 0;
  // printf("\n%d",*(int *)num);
  // printf("\nid:%d \ndesde: %d; hasta: %d\n", desplazamiento, desde, hasta );
  for(int i = desde; i < hasta; i++){
    totalParcial += vector[i];
  }
  p_threads[desplazamiento].total_parcial = totalParcial;

  pthread_exit(0);
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

*/
