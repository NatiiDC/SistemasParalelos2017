#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

/* Globals definition*/
pthread_mutex_t minimum_value_lock;
long i, N, paralelism;
long double timetick;
long *vector;
long debug = 0;

struct My_threads {
	long init;
	long fin;
	long id;
};

/* Functions definition*/
long double dwalltime() {
	long double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

void printVector(long* vector) {
	for (long i = 0; i < N; i++) {
			printf("%ld, ", vector[i]);
	}
	printf("\n");
}

#include <stdio.h>

void qs(long *vector,long limite_izq,long limite_der)
{
    int izq, der, temporal, pivote;
    izq = limite_izq;
    der = limite_der;
		if (der == izq) {
			return;
		}

    pivote = vector[(izq+der)/2];

    do {
        while(vector[izq] < pivote && izq < limite_der) izq++;
        while(pivote < vector[der] && der > limite_izq) der--;
        if(izq <= der)
        {
            temporal = vector[izq];
            vector[izq] = vector[der];
            vector[der] = temporal;
            izq++;
            der--;
        }

    } while(izq <= der);
    if(limite_izq < der) qs(vector, limite_izq, der);
    if(limite_der > izq) qs(vector, izq, limite_der);
}

void quickSort(long *vector, long inicio, long fin) {
		qs(vector, inicio, fin);
}


void mergeIntern(long init, long fin) {
	long *vectorB;
	long N = (fin - init + 1);
	vectorB = (long*)malloc(sizeof(long)*(N));
	long actLeft = init;
	long actRight = init + ( N / 2);
	for(long i=0;i<N;i++){
		if (actLeft == init + ( N / 2)) {
			vectorB[i] = vector[actRight];
			actRight++;
		}
		else {
			if (actRight == fin+1) {
				vectorB[i] = vector[actLeft];
				actLeft++;
			} else {
				if (vector[actLeft] < vector[actRight]) {
					vectorB[i] = vector[actLeft];
					actLeft++;
				} else {
					vectorB[i] = vector[actRight];
					actRight++;
				}

			}
		}
	}
	for(long i=0;i<N;i++) vector[init+i] = vectorB[i];
	free (vectorB);
}

void mergeSort(long init, long fin) {
	if ((fin - init) > 0) {
		long N = fin - init + 1;
	 	long initLeft = init;
		long finLeft = init + (long) N/2 - 1;
		long initRight = finLeft + 1;
		long finRight = fin;
		mergeSort(initLeft, finLeft);
		mergeSort(initRight, finRight);
		mergeIntern(init, fin);
	}
}

void *merge(void* arguments) {
	struct My_threads *p_thread = arguments;
	long init = p_thread->init;
	long fin = p_thread->fin;
	long N = (fin - init + 1);
	long *vectorB = (long*)malloc(sizeof(long)*(N));
	long actLeft = init;
	long actRight = init + ( N / 2);
	for(long i=0;i<N;i++){
		if (actLeft == init + ( N / 2)) {
			vectorB[i] = vector[actRight];
			actRight++;
		}
		else {
			if (actRight == fin+1) {
				vectorB[i] = vector[actLeft];
				actLeft++;
			} else {
				if (vector[actLeft] < vector[actRight]) {
					vectorB[i] = vector[actLeft];
					actLeft++;
				} else {
					vectorB[i] = vector[actRight];
					actRight++;
				}

			}
		}
	}
	for(long i=0;i<N;i++) vector[init+i] = vectorB[i];
	free (vectorB);
}

void *ordenarInicial(void* arguments) {
	struct My_threads *p_thread = arguments;
	quickSort(vector, p_thread->init, p_thread->fin);
}

int main(long argc, char*argv[]) {

	/* Argument validation.*/
	const char* help ="\nCompilar:\n\tgcc -o tp2-ej2 tp2-ej2.c -lm -std=c99 -pthread\nArgumentos:\n\t<E> = 2^E elementos del vector.\n\t<P> = Paralelismo a utilizar.\n\t<S> = Cantidad de muestras a ejecutar.";

	if (argc < 4) {
		printf("%s\n", help);
		return -1;
	}
	if (argc > 4) {
		debug = atoi(argv[4]);
	}

	/* Globals initialization.*/
	long power = atoi(argv[1]);
  N = pow(2, power);
	paralelism = atoi(argv[2]);
	pthread_t p_threads[paralelism];
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	struct My_threads *p_thread = malloc(sizeof(struct My_threads)*paralelism);

	printf("CANTIDAD DE THREADS: %ld.\n", paralelism);
	printf("N: %ld.\n", N);

	vector=(long*)malloc(sizeof(long)*N);

	for(i=0;i<N;i++){
		vector[i] = N - i;
	}

	if (debug) {
		printVector(vector);
	}

	timetick = dwalltime();

	long cantThreads = paralelism;
  for(i = 0; i < paralelism; i++){
		p_thread[i].id = i;
		p_thread[i].init = N/paralelism * i;
		p_thread[i].fin = (N/paralelism * (i+1))-1;
	}
	for(i=0; i < paralelism; i++) {
		pthread_create( &p_threads[i], NULL, ordenarInicial, (void *)&p_thread[i]
		);
	}
	for (i=0; i < paralelism; i++){
		pthread_join(p_threads[i], NULL);
	}

	if (debug) {
		printVector(vector);
		printf("Primer OrdenarInicial\n" );
	}

	for(long cont = 0; cont < (long)log2(paralelism); cont++) {
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

	if (debug) {
		printVector(vector);
	}

}
