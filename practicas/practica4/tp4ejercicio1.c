#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

/* Globals definition*/
long size, edge;
int exponent, identifier, paralelism;
long debug = 0;

long* initMatrix() {
	long element = 0;
	long* matrix = malloc(sizeof(long)*size);
	for (long i = 0; i < size; i++) {
		matrix[i] = 1;
	}
	return matrix;
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


int main(int argc, char *argv[]) {

	const char* help ="\nCompilar en Linux Openmpi:\n\tmpicc -o tp4ejericio1 tp4ejericio.c -lm\nEjecutar en Openmpi:\n\tEn una sola maquina:\n\t\tmpirun -np <P> archivoFuente <E>\n\t\t<P> = cantidad de procesos\n\t\t<E> = 2^E elementos del vector\n\tEn un cluster de máquinas:\n\t\tmpirun -np cantidadDeProcesos -machinefile archivoMaquinas archivoFuente";

	if (argc < 2) {
		printf("%s\n", help);
		return 0;
	}

	/* Globals initialization.*/
	exponent = atoi(argv[1]);
	edge = pow(2, exponent);
	size = edge * edge;
	int tag = 1;

	MPI_Init(&argc, &argv);
	MPI_Status *status;
	MPI_Comm_size (MPI_COMM_WORLD, &paralelism);
	MPI_Comm_rank(MPI_COMM_WORLD, &identifier);

	long *matrix1;
	long *matrix2;
	long *result;
	long *sub_rand_nums;

	if (identifier == 0) {
		matrix1 = initMatrix();
		matrix2 = initMatrix();
		result = calloc(size, sizeof(long));

	}
		// long* matrixOne = malloc(sizeof(long) * size/paralelism);
		// long* matrixTwo = malloc(sizeof(long) * size);
		sub_rand_nums = malloc(sizeof(long) * size/paralelism);

		MPI_Bcast(matrix2, size, MPI_LONG, 0, MPI_COMM_WORLD);
		MPI_Scatter(matrix1, size, MPI_LONG, matrix1, size/paralelism, MPI_LONG, 0, MPI_COMM_WORLD);

		printf("Soy el id: %d\n", identifier);

		MPI_Gather(result, size/paralelism, MPI_LONG, result, MPI_LONG, 0, MPI_COMM_WORLD);

		// MPI_Recv(matrix1, size, MPI_LONG, 0, tag, MPI_COMM_WORLD, status);
		// MPI_Recv(matrix2, size, MPI_LONG, 0, tag, MPI_COMM_WORLD, status);
		// printMatrix(matrix1);
		// printMatrix(matrix2);

	if (identifier == 0) {
		free(matrix1);
		free(matrix2);
		free(result);
	}


	MPI_Finalize();


	return 0;
}
