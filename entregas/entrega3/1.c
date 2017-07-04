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
const char* help ="\nCompilar en Linux Openmpi:\n\tmpicc -o 1 1.c -lm\nEjecutar en Openmpi:\n\tEn una sola maquina:\n\t\tmpirun -np <P> ejecutable <E>\n\t\t<P> = cantidad de procesos\n\t\t<E> = 2^E elementos del lado de la matriz\n\tEn un cluster de máquinas:\n\t\tmpirun -np <P> -machinefile archivoMaquinas ejecutable <E>";

// Data globals.
long long exponent, size, edge, scatteredSize, scatteredEdge;
double communications = 0;
// Id functions.
int isRoot() {
	return id == ROOT;
}

// Debug functions.
int shouldPrint() {
	return debug && isRoot();
}

void printMatrix(double* matrix) {
	for (long long i = 0; i < edge; i++) {
		const long long yOffset = i * edge;
		for (long long j = 0; j < edge; j++) {
			const long long totalOffset = yOffset + j;
			printf("%.3f ", matrix[totalOffset]);
		}
		printf("\n");
	}
	printf("\n");
}

void printScatteredMatrix(double* matrix) {
	for (long long i = 0; i < scatteredEdge; i++) {
		const long long yOffset = i * scatteredEdge;
		for (long long j = 0; j < edge; j++) {
			const long long totalOffset = yOffset + j;
			printf("%.3f ", matrix[totalOffset]);
		}
		printf("\n");
	}
	printf("\n");
}

double stopwatch() {
	double sec;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

// Matrix initialization functions.
double* newMatrix(long long size) {
	return malloc(size * sizeof(double));
}

void freeMatrix(double* matrix) {
	free(matrix);
}

double* initMatrix(long long size) {
	double* matrix = newMatrix(size);
	for (long long i = 0; i < edge; i++) {
		const long long yOffset = i * edge;
		for (long long j = 0; j < edge; j++) {
			const long long totalOffset = yOffset + j;
			if (i == j) {
				matrix[totalOffset] = 1;
			} else {
				matrix[totalOffset] = 0;
			}
		}
	}
	return matrix;
}

double* createMatrix() {
	if (isRoot()) {
		return initMatrix(size);
	}
	return newMatrix(size);
}

double* createAndScatter() {
	double* matrix;
	if (isRoot()) {
		matrix = initMatrix(size);
	}	else {
		matrix = newMatrix(scatteredSize);
	}
	double start = stopwatch();
	MPI_Scatter(matrix, scatteredSize, MPI_DOUBLE,
							matrix, scatteredSize, MPI_DOUBLE,
							ROOT, WORLD);
	double end = stopwatch();
	communications += end - start;
	return matrix;
}

double* createAndBroadcast() {
	double* matrix = createMatrix();
	double start = stopwatch();
	MPI_Bcast(matrix, size, MPI_DOUBLE, ROOT, WORLD);
	double end = stopwatch();
	communications += end - start;
	return matrix;
}

// Matrix operation functions.
// applies over matrix.
void scalarMul(double* matrix, double scalar) {
	for (long long i = 0; i < scatteredSize; i++) {
		matrix[i] = matrix[i] * scalar;
	}
}

double average(double* matrix) {
	double total;
	double partial = 0;
	for (long long i = 0; i < scatteredSize; i++) {
		partial += matrix[i];
	}
	double start = stopwatch();
	MPI_Allreduce(&partial, &total, 1, MPI_DOUBLE, MPI_SUM, WORLD);
	double end = stopwatch();
	communications += end - start;
	return total / size;
}

double* matrixMul(double* lhs, double* rhs) {
	double* result = newMatrix(scatteredSize);
	double partial;
	for(long long i = 0; i < scatteredEdge; i++) {
		const long long lhsOffset = i * edge;
		for(long long j = 0; j < edge; j++) {
			const long long rhsOffset = j * edge;
			const long long totalOffset = lhsOffset + j;
			partial = 0;
			for(long long k = 0; k < edge; k++) {
				partial += lhs[lhsOffset + k] * rhs[rhsOffset + k];
			}
			result[totalOffset] = partial;
		}
	}
	return result;
}

// accumulate over the lhs matrix.
double* matrixSum(double* lhs, double* rhs) {
	for (long long i = 0; i < scatteredSize; i++) {
		lhs[i] += rhs[i];
	}
	double* result = NULL;
	if (isRoot()) {
		result = newMatrix(size);
	}
	double start = stopwatch();
	MPI_Gather(lhs, scatteredSize, MPI_DOUBLE,
						result, scatteredSize, MPI_DOUBLE,
						ROOT, WORLD);
	double end = stopwatch();
	communications += end - start;
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
	edge = pow(2, exponent);
	size = edge * edge;
	scatteredSize = size / paralelism;
	scatteredEdge = edge / paralelism;
	debug = 0;
	if (argc > 2) {
		debug = 1;
	}
	if (shouldPrint()) {
		printf("exponent: %lld\nsize: %lld\n", exponent, size);
	}

	double* a = createAndScatter();
	double* d = createAndScatter();

	double* b = createAndBroadcast();
	double* c = createAndBroadcast();
	double* e = createAndBroadcast();
	double* f = createAndBroadcast();

	double start;
	if (isRoot()) {
		start = stopwatch();
	}

	double* ab = matrixMul(a, b);
	double* abc = matrixMul(ab, c);
	double* de = matrixMul(d, e);
	double* def = matrixMul(de, f);
	double bAVG = average(b);
	double dAVG = average(d);

	scalarMul(abc, dAVG);
	scalarMul(def, bAVG);

	double* result = matrixSum(abc, def);

	if (isRoot()) {
		double end = stopwatch();
		double delta = end - start;
		printf("total seconds: %f\n", delta);
		printf("total communications: %f\n", communications);
		printf("Overhead: %f %%\n", communications / delta);
	}

	if (shouldPrint()) {
		printf("result:\n");
		printMatrix(result);
	}

	freeMatrix(a);
	freeMatrix(b);
	freeMatrix(c);
	freeMatrix(d);
	freeMatrix(e);
	freeMatrix(f);

	freeMatrix(ab);
	freeMatrix(abc);
	freeMatrix(de);
	freeMatrix(def);
	freeMatrix(result);

	MPI_Finalize();

	return 0;
}
