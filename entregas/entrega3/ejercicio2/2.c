#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

// World globals.
int paralelism, id, debug;
const char* help ="\nCompilar en Linux Openmpi:\n\tmpicc -o 1 1.c -lm\nEjecutar en Openmpi:\n\tEn una sola maquina:\n\t\tmpirun -np <P> ejecutable <E>\n\t\t<P> = cantidad de procesos\n\t\t<E> = 2^E elementos del vector\n\tEn un cluster de máquinas:\n\t\tmpirun -np <P> -machinefile archivoMaquinas ejecutable <E>";

// Data globals.
long long exponent, jobs, size, scatteredSize;

#include "common.c"
void master();
void worker();

void startProc() {
	if (isMaster()) {
		master();
	} else {
		worker();
	}
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_size(WORLD, &paralelism);
	MPI_Comm_rank(WORLD, &id);

	if (argc < 2) {
		if (isMaster()) {
			printf("%s\n", help);
		}
		MPI_Finalize();
		return 0;
	}

	exponent = atoi(argv[1]);
	size = pow(2, exponent);
	jobs = pow(2, exponent/2);
	scatteredSize = size / jobs;
	debug = argc > 2;

	if (shouldPrint()) {
		printf("Exponent: %lld\n", exponent);
		printf("Size: %lld\n", size);
		printf("Jobs: %lld\n", jobs);
		printf("Job Size: %lld\n", scatteredSize);
	}
	startProc();

	MPI_Finalize();
	return 0;
}

#include "master.c"
#include "worker.c"
