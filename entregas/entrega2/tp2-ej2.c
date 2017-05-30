#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

/* Globals definition*/
long i, N, paralelism;
long double timetick;
long *vector;
long debug = 0, power;

struct My_threads {
	long init;
	long fin;
	pthread_t thread;
	int id;
};
struct My_threads *p_threads;

// ---------------------------------------------

typedef struct {
	pthread_mutex_t count_lock;
	pthread_cond_t ok_to_proceed;
	int count;
} mylib_barrier_t;
mylib_barrier_t *barriers;


void mylib_init_barrier (mylib_barrier_t* b) {
	b->count = 0;
	pthread_mutex_init(&(b->count_lock), NULL);
	pthread_cond_init(&(b->ok_to_proceed), NULL);
}

void mylib_barrier (mylib_barrier_t *b, int num_threads) {
	pthread_mutex_lock(&(b->count_lock));
	b->count++;
	if (b->count == num_threads) {
		b->count = 0;
		pthread_cond_broadcast(&(b->ok_to_proceed));
	} else {
		pthread_cond_wait(&(b->ok_to_proceed),&(b->count_lock));
	}
	pthread_mutex_unlock(&(b->count_lock));
}

// ----------------------------------------

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

void merge(long init, long fin) {
	long N = (fin - init + 1);
	long *vectorTemp = (long*)malloc(sizeof(long)*(N));
	long actLeft = init;
	long actRight = init + ( N / 2);
	for(long i=0;i<N;i++){
		if (actLeft == init + ( N / 2)) {
			vectorTemp[i] = vector[actRight];
			actRight++;
		}
		else {
			if (actRight == fin+1) {
				vectorTemp[i] = vector[actLeft];
				actLeft++;
			} else {
				if (vector[actLeft] < vector[actRight]) {
					vectorTemp[i] = vector[actLeft];
					actLeft++;
				} else {
					vectorTemp[i] = vector[actRight];
					actRight++;
				}
			}
		}
	}
	for(long i=0;i<N;i++) vector[init+i] = vectorTemp[i];
	free (vectorTemp);
}

void *ordenarInicial(void* args) {
	int p_id = *((int *)args);
	quickSort(vector, p_threads[p_id].init, p_threads[p_id].fin);

	int cant = log2(paralelism);
	if ((p_id%2) == 0) {

		for (int i = 1; i <= cant; i++) {
			if (p_id%(2*i) == 0) {

				int nextMergeId = (int)(p_id+(pow(2, i-1)));
				printf("p_id: %d\twait thread: %d\n",p_id,nextMergeId);
				// pthread_join(( p_threads[(int)(p_id+(pow(2, i-1)))].thread ), NULL);
				mylib_barrier(&barriers[nextMergeId], 2);

				p_threads[p_id].fin = p_threads[nextMergeId].fin;
				printf("p_id: %d -\tthread %d: finish\tfin: %ld\n",p_id, nextMergeId, p_threads[p_id].fin);
				merge(p_threads[p_id].init, p_threads[p_id].fin);
			} else {
				printf("p_id: %d\tIm wait\n",p_id);
				mylib_barrier(&barriers[p_id], 2);
			}
		}
	} else {
		printf("p_id: %d\tIm wait\n",p_id);
		mylib_barrier(&barriers[p_id], 2);
	}
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
	power = atoi(argv[1]);
  N = pow(2, power);
	paralelism = atoi(argv[2]);
	p_threads = malloc(sizeof(struct My_threads) * paralelism);
	barriers = malloc(sizeof(mylib_barrier_t) * (paralelism));

	pthread_attr_t attr;
	pthread_attr_init (&attr);

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
		mylib_init_barrier(&barriers[i]);

		p_threads[i].id = i;
		p_threads[i].init = N/paralelism * i;
		p_threads[i].fin = (N/paralelism * (i+1))-1;
	}
	for(i=0; i < paralelism; i++) {
		pthread_create(&p_threads[i].thread, NULL, ordenarInicial, &p_threads[i].id);
	}
	
	pthread_join(p_threads[0].thread, NULL);
	long double tiempo = dwalltime() - timetick;

	for(i=1; i < paralelism; i++) {
		pthread_join(p_threads[i].thread, NULL);
	}
	printf("Tiempo en segundos %LF \n", tiempo);


	if (debug) {
		printVector(vector);
	}

}
