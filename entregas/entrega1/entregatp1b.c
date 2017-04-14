#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double dwalltime();

void productoEscalar(double x, double* A, int N);
double promedioMatriz(double *M, int n);

void producto(double *A, double *B, double *C, int r, int cantidadBloquesPorLado, int sizeMatrix, int sizeBlock);
void suma(double *A, double *B, int n);
void imprimirMatrizColumna(double* A, int cantidadBloquesPorLado);
void imprimirMatrizFila(double* A, int cantidadBloquesPorLado);


int main( int argc, char* argv[] ) {

	double *A, *B, *C ,*D ,*E ,*F, *AB, *ABC, *DE, *DEF;
	double promB = 0, promD = 0;
	double timetick;
	int i,j;
	int check=1;

	if (argc < 4){
		printf("\n Falta un parametro ");
		printf("\n 1. Cantidad de bloques por lado de la matriz");
		printf("\n 2. Tamaño de bloque");
		printf("\n 3. 1/0 para imprimir/no imprimir resultados\n");
		return 0;
	}

	int cantidadBloquesPorLado = atoi(argv[1]);
	int r = atoi(argv[2]);

	int n = cantidadBloquesPorLado * r; //dimension de la matriz
	int sizeMatrix=n*n; //cantidad total de datos matriz
	int sizeBlock=r*r; //cantidad total de datos del bloque


	//Aloca memoria para las matrices
	A = (double *)malloc(sizeMatrix*sizeof(double));
	B = (double *)malloc(sizeMatrix*sizeof(double));
	C = (double *)malloc(sizeMatrix*sizeof(double));
	D = (double *)malloc(sizeMatrix*sizeof(double));
	E = (double *)malloc(sizeMatrix*sizeof(double));
	F = (double *)malloc(sizeMatrix*sizeof(double));
	AB = (double *)malloc(sizeMatrix*sizeof(double));
	ABC = (double *)malloc(sizeMatrix*sizeof(double));
	DE = (double *)malloc(sizeMatrix*sizeof(double));
	DEF = (double *)malloc(sizeMatrix*sizeof(double));

	//Inicializa las matrices en 1, el resultado sera una matriz con todos sus valores en cantidadBloquesPorLado
	for (int i = 0; i < cantidadBloquesPorLado; i++) {
		for (int j = 0; j < cantidadBloquesPorLado; j++) {
			A[i * cantidadBloquesPorLado + j] = 1;
			B[i + j * cantidadBloquesPorLado] = 1;
			C[i + j * cantidadBloquesPorLado] = 1;
			D[i * cantidadBloquesPorLado + j] = 1;
			E[i + j * cantidadBloquesPorLado] = 1;
			F[i + j * cantidadBloquesPorLado] = 1;
		}
	}

	for (int i = 0; i < cantidadBloquesPorLado; i++) {
		for (int j = 0; j < cantidadBloquesPorLado; j++) {
			AB[i*cantidadBloquesPorLado+j] = 0;
			ABC[i+j*cantidadBloquesPorLado] = 0;
			DE[i*cantidadBloquesPorLado+j] = 0;
			DEF[i+j*cantidadBloquesPorLado] = 0;
		}
	}

	//arrancan las operaciones y el contador
	timetick = dwalltime();

	promB = promedioMatriz(B, cantidadBloquesPorLado);
	promD = promedioMatriz(D, cantidadBloquesPorLado);

	productoEscalar(promD, A, cantidadBloquesPorLado);
	productoEscalar(promB, D, cantidadBloquesPorLado);

	producto(A, B, AB, r, cantidadBloquesPorLado, sizeMatrix, sizeBlock);
	producto(AB, C, ABC, r, cantidadBloquesPorLado, sizeMatrix, sizeBlock);
	producto(D, E, DE, r, cantidadBloquesPorLado, sizeMatrix, sizeBlock);
	producto(DE, F, DEF, r, cantidadBloquesPorLado, sizeMatrix, sizeBlock);

	suma(ABC, DEF, cantidadBloquesPorLado);

	printf("Tiempo en segundos %f \n", dwalltime() - timetick);


	//Verifica el resultado
	for ( i=0; i<cantidadBloquesPorLado; i++) {
		for (j=0; j<cantidadBloquesPorLado; j++) {
			check = check && ( ABC[i*cantidadBloquesPorLado+j] == (2*pow(cantidadBloquesPorLado, 2)) );
		}
	}

	if (check) {
	 printf("Multiplicacion de matrices resultado correcto\n");
	} else {
	 printf("Multiplicacion de matrices resultado erroneo\n");
	}

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
	for(int i=0;i<(N*N);i++){
		A[i] *= x;
	}
}

double promedioMatriz(double *M, int n){
	double total = 0.0;
	int i;

	for (i = 0; i < n*n; i++){
		total += M[i];
	}

	return total / (n*n);
}


void producto(double *A, double *B, double *C, int r, int cantidadBloquesPorLado, int sizeMatrix, int sizeBlock){
	int I,J,K,i,j,k;
	int despA, despB, despC,desp;
	double aux;

	for (i=0; i<sizeMatrix ;i++) {
		C[i]=0.0;
	}

	for (I=0; I<cantidadBloquesPorLado; I++) { // cantidad de bloques.
		for (J=0; J<cantidadBloquesPorLado; J++) {
			despC = (I*cantidadBloquesPorLado+J)*sizeBlock;

			for (K=0; K<cantidadBloquesPorLado; K++) {
				despA = (I*cantidadBloquesPorLado+K) * sizeBlock;
				despB = (K*cantidadBloquesPorLado+J) * sizeBlock;

				for (i=0; i<r; i++) {
					for (j=0; j<r; j++) {
						desp = despC + i*r+j;
						aux = 0;

						for (k=0;k<r;k++){
							aux += A[despA + i*r+k]*B[despB + k*r+j];
						};

						C[desp] += aux;
					}
				};
			};
		};
	};
}

void suma(double *A, double *B, int N) {
	for(int i=0; i<(N*N); i++) {
		A[i] += B[i];
	}
}

void imprimirMatrizFila(double* A, int cantidadBloquesPorLado) {
	printf("\n");
	for(int i=0; i<cantidadBloquesPorLado; i++){
		for(int j=0; j<cantidadBloquesPorLado; j++){
			printf("%d ", (int) A[i*cantidadBloquesPorLado+j]);
		}
		printf("\n");
	}
	printf("\n");
}

void imprimirMatrizColumna(double* A, int cantidadBloquesPorLado) {
	printf("\n");
	for(int i=0;i<cantidadBloquesPorLado;i++){
		for(int j=0;j<cantidadBloquesPorLado;j++){
			printf("%d ", (int) A[i+j*cantidadBloquesPorLado]);
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
