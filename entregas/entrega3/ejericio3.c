#include <sys/time.h>
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

// World globals.
int paralelism, id, debug;
const char* help ="\nCompilar en Linux Openmpi:\n\tmpicc -o 1 1.c -lm\nEjecutar en Openmpi:\n\tEn una sola maquina:\n\t\tmpirun -np <P> ejecutable <E>\n\t\t<P> = cantidad de procesos\n\t\t<E> = 2^E elementos del vector\n\tEn un cluster de máquinas:\n\t\tmpirun -np cantidadDeProcesos -machinefile archivoMaquinas ejecutable";

// Data globals.
long long exponent, size, scatteredSize;
struct Map {
  int num;
  long occu;
};

struct Map *occurrences;

// Id functions.
int isRoot() {
	return id == ROOT;
}

// Debug functions.
int shouldPrint() {
	return debug && isRoot();
}

void printArray(int* arr){
  for (size_t i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int* newArray(long long size) {
  return malloc(size * sizeof(int));
}

void freeArray(int* array) {
	free(array);
}

int* initArray(long long size){
  int* array = newArray(size);
  // srand(time(NULL));
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
  MPI_Scatter(array, scatteredSize, MPI_INT,
              array, scatteredSize, MPI_INT,
              ROOT,WORLD);
  return array;
}

double stopwatch() {
	double sec;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
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

  int* arr = createArray();

  if (shouldPrint()) {
		printf("result: \n");
		printArray(arr);
	}

  MPI_Finalize();
  return 0;
}
