#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double dwalltime();

void productoEscalar(double x, double* A, int N);
double promedioMatriz(double *M, int n);
void producto(double *A, double *B, double *C, int cantLadoBloque, int cantBloquesPorLado, int sizeMatrix, int sizeBlock);
void suma(double *A, double *B, int n);
void imprimirMatrizColumna(double* A, int cantBloquesPorLado);
void imprimirMatrizFila(double* A, int cantBloquesPorLado);

int main( int argc, char* argv[] ) {

	double *A, *B, *C ,*D ,*E ,*F, *AB, *ABC, *DE, *DEF;
	double promB = 0, promD = 0;
	double timetick;
	int iter = 1;

	if (argc < 3){
		printf("\n Falta un parametro ");
		printf("\n 1. Cantidad de bloques por lado de la matriz");
		printf("\n 2. Tamaño del lado bloque");
		printf("\n 3 (opcional) cantidad de iteraciones");
		return 0;
	}

	int cantBloquesPorLado = atoi(argv[1]);
	int cantLadoBloque = atoi(argv[2]);

	if (argc == 4) {
		iter = atoi(argv[3]);
	}

	int n = cantBloquesPorLado * cantLadoBloque; //dimension de la matriz
	int sizeMatrix = n * n; //cantidad total de datos matriz
	int sizeBlock = cantLadoBloque * cantLadoBloque; //cantidad total de datos del bloque

	//Aloca memoria para las matrices
	A = (double *)malloc(sizeMatrix*sizeof(double));
	B = (double *)malloc(sizeMatrix*sizeof(double));
	C = (double *)malloc(sizeMatrix*sizeof(double));
	D = (double *)malloc(sizeMatrix*sizeof(double));
	E = (double *)malloc(sizeMatrix*sizeof(double));
	F = (double *)malloc(sizeMatrix*sizeof(double));
	AB = (double *)calloc(sizeMatrix, sizeof(double));
	ABC = (double *)calloc(sizeMatrix, sizeof(double));
	DE = (double *)calloc(sizeMatrix, sizeof(double));
	DEF = (double *)calloc(sizeMatrix, sizeof(double));

	//Inicializa las matrices en 1, el resultado sera una matriz con todos sus valores en cantBloquesPorLado
	for (int i = 0; i < cantBloquesPorLado; i++) {
		for (int j = 0; j < cantBloquesPorLado; j++) {
			A[i * cantBloquesPorLado + j] = 1;
			B[i + j * cantBloquesPorLado] = 1;
			C[i + j * cantBloquesPorLado] = 1;
			D[i * cantBloquesPorLado + j] = 1;
			E[i + j * cantBloquesPorLado] = 1;
			F[i + j * cantBloquesPorLado] = 1;
		}
	}

	double promedio = 0;
	for (int a = 0; a < iter; a++) {
		//arrancan las operaciones y el contador
		timetick = dwalltime();

		promB = promedioMatriz(B, cantBloquesPorLado);
		productoEscalar(promB, D, cantBloquesPorLado);

		promD = promedioMatriz(D, cantBloquesPorLado);
		productoEscalar(promD, A, cantBloquesPorLado);

		producto(A, B, AB, cantLadoBloque, cantBloquesPorLado, sizeMatrix, sizeBlock);
		producto(AB, C, ABC, cantLadoBloque, cantBloquesPorLado, sizeMatrix, sizeBlock);
		producto(D, E, DE, cantLadoBloque, cantBloquesPorLado, sizeMatrix, sizeBlock);
		producto(DE, F, DEF, cantLadoBloque, cantBloquesPorLado, sizeMatrix, sizeBlock);

		suma(ABC, DEF, cantBloquesPorLado);

		promedio += dwalltime() - timetick;
	}
	promedio = promedio / iter;
	printf("Tiempo promedio para %d iteracion/es: %f \n", iter, promedio);

	free(A);
	free(B);
	free(C);
	free(D);
	free(E);
	free(F);
	free(AB);
	free(ABC);
	free(DE);
	free(DEF);

}

void productoEscalar(double x, double* A, int N) {
	for(int i = 0; i < (N*N); i++){
		A[i] *= x;
	}
}

double promedioMatriz(double *M, int n){
	double total = 0.0;
	for (int i = 0; i < n*n; i++){
		total += M[i];
	}
	return total / (n*n);
}


void producto(double *A, double *B, double *C, int cantLadoBloque, int cantBloquesPorLado, int sizeMatrix, int sizeBlock){
	int I,J,K,i,j,k;
	int despA, despB, despC,desp;
	double temp = 0;

	for (I = 0; I < cantBloquesPorLado; I++) { // cantidad de bloques.
		for (J = 0; J < cantBloquesPorLado; J++) {
			despC = (I*cantBloquesPorLado + J) * sizeBlock;

			for (K = 0; K < cantBloquesPorLado; K++) {
				despA = (I*cantBloquesPorLado + K) * sizeBlock;
				despB = (K*cantBloquesPorLado + J) * sizeBlock;

				for (i = 0; i < cantLadoBloque; i++) {
					for (j = 0; j < cantLadoBloque; j++) {
						desp = despC + i*cantLadoBloque + j;

						for (k = 0; k < cantLadoBloque; k++){
							temp += A[despA + i*cantLadoBloque + k] * B[despB + k*cantLadoBloque + j];
						};

						C[desp] += temp;
						temp = 0;
					}
				}
			}
		}
	}
}

void suma(double *A, double *B, int N) {
	for(int i=0; i<(N*N); i++) {
		A[i] += B[i];
	}
}

void imprimirMatrizFila(double* A, int cantBloquesPorLado) {
	printf("\n");
	for(int i=0; i<cantBloquesPorLado; i++){
		for(int j=0; j<cantBloquesPorLado; j++){
			printf("%d ", (int) A[i*cantBloquesPorLado+j]);
		}
		printf("\n");
	}
	printf("\n");
}

void imprimirMatrizColumna(double* A, int cantBloquesPorLado) {
	printf("\n");
	for(int i=0;i<cantBloquesPorLado;i++){
		for(int j=0;j<cantBloquesPorLado;j++){
			printf("%d ", (int) A[i+j*cantBloquesPorLado]);
		}
		printf("\n");
	}
	printf("\n");
}

/*****************************************************************/

#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
