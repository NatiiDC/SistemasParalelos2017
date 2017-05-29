#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

/* Globals definition*/
pthread_mutex_t minimum_value_lock;
size_t i, N, paralelism;
long double timetick;
size_t *A;

struct My_threads {
	size_t init;
	size_t fin;
};

/* Functions definition*/
long double dwalltime() {
	long double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

void *ordenarInicial(void* arguments) {
	struct My_threads *p_thread = arguments;
	if ((p_thread->fin - p_thread->init) > 0) {
		long long N = (p_thread->fin - p_thread->init + 1);
	 	long long initLeft = p_thread->init;
		long long finLeft = (p_thread->init + (long long)N/2) - 1;
		long long initRight = finLeft + 1;
		long long finRight = p_thread->fin;
		mergeSort(initLeft, finLeft);
		mergeSort(initRight, finRight);
		merge(p_thread);
	}
}

void mergeSort(long long init, long long fin) {
	if ((fin - init) > 0) {
		long long N = fin - init + 1;
	 	long long initLeft = init;
		long long finLeft = init + (long long) N/2 - 1;
		long long initRight = finLeft + 1;
		long long finRight = fin;
		mergeSort(initLeft, finLeft);
		mergeSort(initRight, finRight);
		mergeIntern(init, fin);
	}
}

void *merge(void* arguments) {
	struct My_threads *p_thread = arguments;
	long long *B;
	long long init = p_thread->init;
	long long fin = p_thread->fin;
	long long N = (fin - init + 1);
	B = (long long*)malloc(sizeof(long long)*(N));
	long long actLeft = init;
	long long actRight = init + ( N / 2);
	for(long long i=0;i<N;i++){
		if (actLeft == init + ( N / 2)) {
			B[i] = A[actRight];
			actRight++;
		}
		else {
			if (actRight == fin+1) {
				B[i] = A[actLeft];
				actLeft++;
			} else {
				if (A[actLeft] < A[actRight]) {
					B[i] = A[actLeft];
					actLeft++;
				} else {
					B[i] = A[actRight];
					actRight++;
				}

			}
		}
	}
	for(long long i=0;i<N;i++) A[init+i] = B[i];
	free (B);
}

void mergeIntern(long long init, long long fin) {
	long long *B;
	long long N = (fin - init + 1);
	B = (long long*)malloc(sizeof(long long)*(N));
	long long actLeft = init;
	long long actRight = init + ( N / 2);
	for(long long i=0;i<N;i++){
		if (actLeft == init + ( N / 2)) {
			B[i] = A[actRight];
			actRight++;
		}
		else {
			if (actRight == fin+1) {
				B[i] = A[actLeft];
				actLeft++;
			} else {
				if (A[actLeft] < A[actRight]) {
					B[i] = A[actLeft];
					actLeft++;
				} else {
					B[i] = A[actRight];
					actRight++;
				}

			}
		}
	}
	for(long long i=0;i<N;i++) A[init+i] = B[i];
	free (B);
}

int main(long long argc, char*argv[]) {

	/* Argument validation.*/
	const char* help ="\nCompilar:\n\tgcc -o tp2-ej2 tp2-ej2.c -lm -std=c99 -pthread\nArgumentos:\n\t
	<E> = 2^E elementos del vector.\n\t
	<P> = Paralelismo a utilizar.\n\t
	<S> = Cantidad de muestras a ejecutar.";

	if (argc < 4) {
		printf("%s\n", help);
		return -1;
	}

	/* Globals initialization.*/
	size_t power = atoi(argv[1]);
  N = pow(2, power);
	paralelism = atoi(argv[2]);

	printf("CANTIDAD DE THREADS: %lld.\n", paralelism);
	printf("N: %lld.\n", N);

	A=(long long*)malloc(sizeof(long long)*N);

	for(i=0;i<N;i++){
		A[i] = N - i;
	}

	pthread_t p_threads[paralelism];
	pthread_attr_t attr;
	pthread_attr_init (&attr);

	struct My_threads *p_thread = malloc(sizeof(struct My_threads)*paralelism);

	long long cantThreads = paralelism;
	timetick = dwalltime();

  for(i = 0; i < paralelism; i++){
		p_thread[i].init = N/paralelism * i;
		p_thread[i].fin = (N/paralelism * (i+1))-1;
	}
	for(i=0; i<paralelism; i++) {
		pthread_create( &p_threads[i], NULL, ordenarInicial, (void *)&p_thread[i]
		);
	}
	for (i=0; i< paralelism; i++){
		pthread_join(p_threads[i], NULL);
	}

	for(long long cont = 0; cont < (long long)log2(paralelism); cont++) {
		cantThreads = cantThreads/2;
		for(i = 0; i < cantThreads; i++){
			p_thread[i].init = N/cantThreads * i;
			p_thread[i].fin = (N/cantThreads * (i+1))-1;
		}
		for(i=0; i<cantThreads; i++) {
			pthread_create(&p_threads[i], NULL, merge, (void *)&p_thread[i]);
		}
		for (i=0; i< cantThreads; i++){
			pthread_join(p_threads[i], NULL);
		}
	}

	long double tiempo = dwalltime() - timetick;
	printf("Tiempo en segundos %LF \n", tiempo);

}
