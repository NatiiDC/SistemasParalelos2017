#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

/* Time in seconds from some point in the past */
long double dwalltime();

int count_threads = 2, find, iter = 1, tot;
long *vector;
long long N;
double timetick;

int main(int argc, char const *argv[]) {

  if (argc < 3) {
    printf("./tp2-ej4.c tamañoDelVector numero [4]\n");
    printf("Debes ingresar el tamaño del vecto y numero a encontrar (0 <= x <= 100)\n");
    printf("Parametro opcional 2 o 4 threads\n");
    printf("Como compilar: gcc -fopenmp -o tp2-ej4 tp2-ej4.c -lm\n");
    return 1;
  }

  if (argc > 3 && atoi(argv[3]) == 4) {
    count_threads = 4;
  }
  omp_set_num_threads(count_threads);

  if (argc == 5) {
    iter = atoi(argv[4]);
  }

  N = atol(argv[1]) * atol(argv[1]);
  find = atoi(argv[2]);
  int tot;
  vector = (long*)malloc(sizeof(long) * N);
  for (int it = 0; it < N; it++) {
    vector[it] = ((it*2)+(it*3))%10000;
  }

  printf("CANTIDAD DE THREADS: %d.\n", count_threads);

  long double promedio = 0.0;
  for (int a = 0; a < iter; a++) {

    tot = 0;
    timetick = dwalltime();

    #pragma omp parallel for reduction(+: tot)
    for(int i = 0; i < N; i++){
      if (vector[i] == find) {
        tot++;
      }
    }

    promedio += dwalltime() - timetick;

  }
  promedio = promedio / iter;
  printf("%d elementos encontrados.\n", tot);
  printf("Tiempo promedio para %LF iteracion/es: %d  \n",promedio, iter);

  return 0;
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
