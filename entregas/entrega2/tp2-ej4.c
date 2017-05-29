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
    printf("Debes ingresar el tamaño del vector (2 ^ N) y numero a encontrar (0 <= x <= 100)\n");
    printf("Parametro opcional 2 o 4 threads\n");
    printf("Como compilar: gcc -fopenmp -o tp2-ej4 tp2-ej4.c -lm -std=c99\n");
    return 1;
  }

  if (argc > 3) {
    count_threads = atoi(argv[3]);
  }
  omp_set_num_threads(count_threads);

  if (argc == 5) {
    iter = atoi(argv[4]);
  }
  long long power = atol(argv[1]);
  N = pow(2, power);
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
    long double tiempo = dwalltime() - timetick;
    printf("%d tardó %LF\n", a, tiempo);
    promedio += tiempo;
  }
  promedio = promedio / iter;
  printf("%d elementos encontrados.\n", tot);
  printf("Tiempo promedio para %d iteracion/es: %LF  \n",iter, promedio);
  free(vector);

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
