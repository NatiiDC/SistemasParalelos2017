ejer3.c
DETALLES
ACTIVIDAD
Hoy

Nicolás Ferella subió un elemento
17:00
C
ejer3.c
No hay actividad registrada antes del 12 de mayo de 2017

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double dwalltime();
void imprimirInferior(int* A, int N);
void imprimirSuperior(int* A, int N);

int main (int argc,char* argv[]){
	int i,hilos,E,N;
	int *A,*T;
	if ((argc < 3) || ((N = atoi(argv[1])) <= 0) || ((hilos = atoi(argv[2])) < 1) )	{
	    printf("\nUsar: %s n\n n: Dimension de la matriz (n x n)\n T: cantidad threads\n Para imprimir 1 (opcional)\n", argv[0]);
	    exit(1);
	}

	E = N*(N+1)/2;
	A=(int*)malloc(sizeof(int)*E);
	T=(int*)malloc(sizeof(int)*E);
	for (int i = 0; i < E; ++i) {
		A[i] = i+1;
		T[i] = 0;
	}
	if (argc == 4) {
		if (atoi(argv[3]) == 1) {
		    printf("Matriz Inicial\n");
			imprimirInferior(A,N);
		}
	}
	double timetick = dwalltime();
	omp_set_num_threads(hilos);

	#pragma omp parallel for
	for (int i = 0; i < E; ++i) {
		int column = i;
		int row = 0;
		while (column >= 0) {
			column-=row+1;
			row++;
		}
		row--;
		column = i - (row*(row+1)/2);
		int pos = (N * column) + row - ((column * (column+1)) / 2);
		T[pos] = A[i];
	}
	double tiempo = dwalltime() - timetick;
	if (argc == 4) {
		if (atoi(argv[3]) == 1) {
		    printf("Matriz Resultante\n");
			imprimirSuperior(T,N);
		}
	}
	printf("Tiempo en segundos %f \n", tiempo );
}

void imprimirInferior(int* A, int N) {
	int actual = 0;
	for(int i=0;i<N;i++){
		for(int j=0;j<i+1;j++){
			printf("%d ", (int) A[actual]);
			actual++;
		}
		for(int j=i+1;j<N;j++){
			printf("0 ");
		}
		printf("\n");
	}
	printf("\n");
}

void imprimirSuperior(int* A, int N) {
	int actual = 0;
	for(int i=0;i<N;i++){
		for(int j=0;j<i;j++){
			printf("0 ");
		}
		for(int j=i;j<N;j++){
			printf("%d ", (int) A[actual]);
			actual++;
		}
		printf("\n");
	}
	printf("\n");
}

#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
