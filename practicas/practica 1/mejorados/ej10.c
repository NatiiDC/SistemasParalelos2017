#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Time in seconds from some point in the past */
double dwalltime();

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Debe ingresar el tamaño del arreglo (potencia de 2)\ny");
        printf("./ej10 n\n");
        return 1;
    }

    double timetick;
    unsigned long long N = atoi(argv[1]);
    double *v;
    v = (double*) malloc(sizeof(double) * N);
    
    srand(time(NULL));

    unsigned long long aux;
    for (int i = 0; i < N; i++) {
        aux = rand()%2000;
        if (aux == 0) aux = 250;
        v[i] = aux;
    }

	timetick = dwalltime();

    int i,j;
    for (i = 2; i <= N; i*=2) {
        j = i/2;
        for (int k = 0; k < N; k+=i) {
            v[k] /= v[k+j];
            printf("%d %f\n", k, v[k]);
        }
    }

    printf("Tiempo en segundos %f \n", dwalltime() - timetick);
    printf("Resultado: %f\n", v[0]);

    return 0;
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
