#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

/* Globals definition*/
long size, edge;
int exponent, identifier, paralelism;
long debug = 0;

void initMatrix(long* matrix) {
	for (long i = 0; i < size; i++) {
		matrix[i] = 1;
	}
}

void printMatrix(long* matrix) {
	for (long rows = 0; rows < edge; rows++) {
		long yOffset = rows * edge;
		for (long columns = 0; columns < edge; columns++) {
			long totalOffset = yOffset + columns;
			printf("%ld, ", matrix[totalOffset]);
		}
		printf("\n");
	}
}

void producto(long* matrixA, long* matrixB, long* result) {
	long aux;
	for (long i = 0; i < edge/paralelism; i++) {
		for (long j = 0; j < edge; j++) {
			aux = 0;
			for (long k = 0; k < edge; k++) {
				aux = aux + matrixA[i*edge + k] * matrixB[k + j*edge];
			}
			result[i*edge+j] = aux;
		}
	}
}

int main(int argc, char *argv[]) {

	const char* help ="\nCompilar en Linux Openmpi:\n\tmpicc -o tp4ejericio1 tp4ejercicio1.c -lm\nEjecutar en Openmpi:\n\tEn una sola maquina:\n\t\tmpirun -np <P> archivoFuente <E>\n\t\t<P> = cantidad de procesos\n\t\t<E> = 2^E elementos del vector\n\tEn un cluster de máquinas:\n\t\tmpirun -np cantidadDeProcesos -machinefile archivoMaquinas archivoFuente";

	if (argc < 2) {
		printf("%s\n", help);
		return 0;
	}

	/* Globals initialization.*/
	exponent = atoi(argv[1]);
	edge = pow(2, exponent);
	size = edge * edge;
	// int tag = 1;

	MPI_Init(&argc, &argv);
	// MPI_Status *status;
	MPI_Comm_size (MPI_COMM_WORLD, &paralelism);
	MPI_Comm_rank(MPI_COMM_WORLD, &identifier);

	long *matrix1;
	long *matrix2 = malloc(sizeof(long)*size);
	long *result = calloc(size, sizeof(long));
	long *sub_rand_nums = malloc(sizeof(long)*(size/paralelism));

	if (identifier == 0) {
		matrix1 = malloc(sizeof(long)*size);
		initMatrix(matrix1);
		initMatrix(matrix2);
	} else {
		matrix1 = malloc(sizeof(long)*(size/paralelism));
	}

		MPI_Bcast(matrix2, size, MPI_LONG, 0, MPI_COMM_WORLD);
		MPI_Scatter(matrix1, (int)size/paralelism, MPI_LONG, matrix1, (int)size/paralelism, MPI_LONG, 0, MPI_COMM_WORLD);

		producto(matrix1, matrix2, sub_rand_nums);

		MPI_Gather(sub_rand_nums, (int)size/paralelism, MPI_LONG, result, (int)size/paralelism, MPI_LONG, 0, MPI_COMM_WORLD);


	if (identifier == 0) {
		printMatrix(result);
		free(matrix1);
		free(matrix2);
		free(sub_rand_nums);
		free(result);
	}


	MPI_Finalize();


	return 0;
}
