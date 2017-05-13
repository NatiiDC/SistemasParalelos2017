#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

pthread_mutex_t minimum_value_lock;

struct My_threads {
	int init;
	int fin;
};

/* Time in seconds from some point in the past */
long double dwalltime();
void *merge(void* arguments);
void mergeSort(int init, int fin);
void mergeIntern(int init, int fin);
void *ordenarInicial(void* arguments);

int i,N,count_threads;
long double timetick;
int *A;

int main(int argc, char*argv[]) {

	if (argc < 2) {
		printf("\n Falta un argumento: N dimension del vector \n");
		printf("\n 2do argumento opcional: cantidad de hilos \n");
		printf("\n Compilar:\ngcc -pthread -o tp2-ej2 tp2-ej2.c -lm\n");

		return 0;
	}

  N = atoi(argv[1]) * atoi(argv[1]);
	if (argc >= 3 && atoi(argv[2]) == 4) {
		count_threads = 4;
	}
	printf("CANTIDAD DE THREADS: %d.\n", count_threads);
	printf("N: %d.\n", N);

	A=(int*)malloc(sizeof(int)*N);

	for(i=0;i<N;i++){
		A[i] = N - i;
	}

	pthread_t p_threads[count_threads];
	pthread_attr_t attr;
	pthread_attr_init (&attr);

  if (pthread_mutex_init(&minimum_value_lock, NULL) != 0)
  {
      printf("\n mutex init failed\n");
      return 1;
  }

	struct My_threads *p_thread = malloc(sizeof(struct My_threads)*count_threads);

	int cantThreads = count_threads;
	timetick = dwalltime();

  for(i = 0; i < count_threads; i++){
		p_thread[i].init = N/count_threads * i;
		p_thread[i].fin = (N/count_threads * (i+1))-1;
	}
	for(i=0; i<count_threads; i++) {
		pthread_create(
			&p_threads[i],
			NULL,
			ordenarInicial,
			(void *)&p_thread[i]
		);
	}
	for (i=0; i< count_threads; i++){
		pthread_join(p_threads[i], NULL);
	}

	for(int cont = 0; cont < (int)log2(count_threads); cont++){
		cantThreads = cantThreads/2;
		for(i = 0; i < cantThreads; i++){
			p_thread[i].init = N/cantThreads * i;
			p_thread[i].fin = (N/cantThreads * (i+1))-1;
		}
		for(i=0; i<cantThreads; i++) {
			pthread_create(
				&p_threads[i],
				NULL,
				merge,
				(void *)&p_thread[i]
			);
		}
		for (i=0; i< cantThreads; i++){
			pthread_join(p_threads[i], NULL);
		}
	}


	double tiempo = dwalltime() - timetick;
	printf("Tiempo en segundos %f \n", tiempo );
}

/***************************  functions **************************************/

void *ordenarInicial(void* arguments) {
	struct My_threads *p_thread = arguments;
	if ((p_thread->fin - p_thread->init) > 0) {
		int N = (p_thread->fin - p_thread->init + 1);
	 	int initLeft = p_thread->init;
		int finLeft = (p_thread->init + (int)N/2) - 1;
		int initRight = finLeft + 1;
		int finRight = p_thread->fin;
		mergeSort(initLeft, finLeft);
		mergeSort(initRight, finRight);
		merge(p_thread);
	}
}

void mergeSort(int init, int fin) {
	if ((fin - init) > 0) {
		int N = fin - init + 1;
	 	int initLeft = init;
		int finLeft = init + (int) N/2 - 1;
		int initRight = finLeft + 1;
		int finRight = fin;
		mergeSort(initLeft, finLeft);
		mergeSort(initRight, finRight);
		mergeIntern(init, fin);
	}
}

void *merge(void* arguments) {
	struct My_threads *p_thread = arguments;
	int *B;
	int init = p_thread->init;
	int fin = p_thread->fin;
	int N = (fin - init + 1);
	B = (int*)malloc(sizeof(int)*(N));
	int actLeft = init;
	int actRight = init + ( N / 2);
	for(int i=0;i<N;i++){
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
	for(int i=0;i<N;i++) A[init+i] = B[i];
	free (B);
}

void mergeIntern(int init, int fin) {
	int *B;
	int N = (fin - init + 1);
	B = (int*)malloc(sizeof(int)*(N));
	int actLeft = init;
	int actRight = init + ( N / 2);
	for(int i=0;i<N;i++){
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
	for(int i=0;i<N;i++) A[init+i] = B[i];
	free (B);
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
