#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

/* Globals definition*/
size_t exponent = 24;
size_t edge;
size_t size;
size_t paralelism = 1;
size_t iterations = 10;
long double totalTime = 0.0;
size_t debug = 0;

long double dwalltime() {
	long double sec;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

size_t* initMatrix() {
	size_t element = 0;
	size_t* matrix = calloc(size, sizeof(size_t));
	for (size_t rows = 0; rows < edge; rows++) {
		size_t yOffset = rows * edge;
		for (size_t columns = 0; columns <= rows; columns++) {
			size_t totalOffset = yOffset + columns;
			matrix[totalOffset] = ++element;
		}
	}
	return matrix;
}

void printMatrix(size_t* matrix) {
	for (size_t rows = 0; rows < edge; rows++) {
		size_t yOffset = rows * edge;
		for (size_t columns = 0; columns < edge; columns++) {
			size_t totalOffset = yOffset + columns;
			printf("%zu, ", matrix[totalOffset]);
		}
		printf("\n");
	}
}

int main(int argc, char const *argv[]) {

	/* Argument validation.*/
	const char* help ="\nCompilar:\n\tgcc -o ej3 ej3.c -lm -std=c99 -fopenmp\nArgumentos:\n\t
	<E> = 2^E elementos por lado de la matriz.\n\t<P> = Paralelismo a utilizar.\n\t
	<S> = Cantidad de muestras a ejecutar.";

	if (argc < 4) {
		printf("%s\n", help);
		return -1;
	}

	/* Globals initialization.*/
	exponent = atoi(argv[1]);
	edge = pow(2, exponent);
	size = edge * edge;

	paralelism = atoi(argv[2]);
	omp_set_num_threads(paralelism);

	iterations = atoi(argv[3]);
	if (argc > 4) {
		debug = atoi(argv[4]);
	}

	/* Main loop.*/
	for (size_t round = 0; round < iterations; round++) {
		size_t* matrix = initMatrix();
		long double start = dwalltime();
		/* Start race.*/
		#pragma omp parallel for schedule(dynamic, 1)
		//#pragma omp parallel for schedule(guided)
		for (size_t rows = edge - 1; rows > 0; rows--) { // Backwards
		//for (size_t rows = 0; rows < edge; rows++) { // Forward
			size_t yOffset = rows * edge;
			for (size_t columns = 0; columns <= rows; columns++) {
				size_t originalOffset = yOffset + columns;
				size_t transposedOffset = (columns * edge) + rows;
				if (originalOffset != transposedOffset) {
					matrix[transposedOffset] = matrix[originalOffset];
					matrix[originalOffset] = 0;
				}
			}
		}
		/* End race.*/
		long double end = dwalltime();
		long double delta = end - start;
		totalTime+= delta;

		if (debug) {
			printMatrix(matrix);
		}
		free(matrix);
	}
	long double average = totalTime / iterations;
	printf("Average for edge of 2^%zu with %zu processors: %LF seconds.\n", exponent, paralelism, average);
	return 0;
}
