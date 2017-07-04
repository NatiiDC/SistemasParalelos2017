#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#ifndef ROOT
#define ROOT 0
#endif
#ifndef WORLD
#define WORLD MPI_COMM_WORLD
#endif

#define TRUE 1
#define FALSE 0

// World globals.
int paralelism, id, debug;
long iteraciones;
MPI_Status status;
const char* help ="\nCompilar en Linux Openmpi:\n\tmpicc -o 1 1.c -lm\nEjecutar en Openmpi:\n\tEn una sola maquina:\n\t\tmpirun -np <P> ejecutable <E>\n\t\t<P> = cantidad de procesos\n\t\t<E> = 2^E elementos del vector\n\tEn un cluster de máquinas:\n\t\tmpirun -np cantidadDeProcesos -machinefile archivoMaquinas ejecutable";

// Data globals.
long long exponent, size, scatteredSize;
double communications = 0;

int isRoot() {
	return id == ROOT;
}

// Debug functions.
int shouldPrint() {
	return debug && isRoot();
}

void printArray(int* arr, long long size){
	for (long long i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}
// --------
double stopwatch() {
	double sec;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

int* newArray(long long size) {
	return malloc(size * sizeof(int));
}

void freeArray(int* array) {
	free(array);
}

int* initArray(long long size){
	int* array = newArray(size);
	time_t t;
	srand((unsigned)time(&t));
	for (size_t i = 0; i < size; i++) {
		array[i] = rand() % size/2;
	}
	return array;
}

int* createArray() {
	int* array;
	if (isRoot()) {
		array = initArray(size);
	} else {
		array = newArray(scatteredSize);
	}
	double start = stopwatch();
	MPI_Scatter(array, scatteredSize, MPI_INT,
							array, scatteredSize, MPI_INT,
							ROOT, WORLD);
	double end = stopwatch();
	communications += end - start;
	return array;
}

// --------

int is_receiver(int i) {
	if ((id % (int)pow(2, i)) == 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

int is_sender(int i) {
	if ((id % (int)pow(2, i)) == (int)pow(2, i-1)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

int receiver(int i) {
	return (int)(id-(pow(2, i-1)));
}

int sender(int i) {
	return (int)(id+(pow(2, i-1)));
}

int* merge(int* L, int* R, int size) {
	int* vectorTemp = newArray(size*2);
	long i,j,k;
	i = 0; j = 0; k = 0;

	while(i < size && j < size) {
		if(L[i] < R[j]) vectorTemp[k++] = L[i++];
		else vectorTemp[k++] = R[j++];
	}
	while(i < size) vectorTemp[k++] = L[i++];
	while(j < size) vectorTemp[k++] = R[j++];

	return vectorTemp;
}

int comparison_int(const void *e1, const void *e2) {
		int a = *((int*) e1);
		int b = *((int*) e2);
		if (a < b) {
				return -1;
		} else if (a > b) {
				return 1;
		} else {
				return 0;
		}
}

int* sort(int* arr) {
		qsort(arr, scatteredSize, sizeof(int), comparison_int);
		int* result = arr;
		int block = scatteredSize;
		int* recive = newArray(block);
		for(int i = 1; i <= log2(paralelism); i++) {
			if (is_sender(i)) {
				// printf("Soy: %d - Mando a: %d\n",id, receiver(i) );
				double start = stopwatch();
				MPI_Send(arr, block, MPI_INT, receiver(i), 0, MPI_COMM_WORLD);
				double end = stopwatch();
				communications += end - start;
			} else if (is_receiver(i)) {
				// printf("Soy: %d - Recibo de: %d\n",id, sender(i) );
				double start = stopwatch();
				MPI_Recv(recive, block, MPI_INT, sender(i), 0, MPI_COMM_WORLD, &status);
				double end = stopwatch();
				communications += end - start;
				result = merge(arr, recive, block);
				block *= 2;
				arr = result;
			}
		}
		return result;
}



int main(int argc, char *argv[]) {

	MPI_Init(&argc, &argv);
	MPI_Comm_size(WORLD, &paralelism);
	MPI_Comm_rank(WORLD, &id);

	if (argc < 2) {
		if (isRoot()) {
			printf("%s\n", help);
		}
		MPI_Finalize();
		return 0;
	}

	/* Data Globals initialization.*/
	exponent = atoi(argv[1]);
	size = pow(2, exponent);
	scatteredSize = size / paralelism;
	debug = 0;
	if (argc > 2) {
		debug = 1;
	}
	if (shouldPrint()) {
		printf("exponent: %lld\nsize: %lld\n", exponent, size);
	}

	double start;
	if (isRoot()) {
		start = stopwatch();
	}

	// sort
	int* arr = createArray();
	int* arrSort = sort(arr);
	freeArray(arr);

	if (isRoot()) {
		double end = stopwatch();
		double delta = end - start;
		printf("total seconds: %f\n", delta);
		printf("total communications: %f\n", communications);
		printf("Overhead: %f %%\n", communications / delta);
	}

	MPI_Finalize();
	return 0;
}
