#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

/* Time in seconds from some point in the past */
long double dwalltime();

void productoEscalar(double x, double* A, int N);
double promedioMatriz(double *M, int n);
void producto(double *ret, double *pri, double *seg, int n);
void suma(double *A, double *B, int n);

void * pthread_productoEscalar(void* args);
void * pthread_promedioMatriz(void* args);
void * pthread_producto(void* args);
void * pthread_suma(void* args);

double *A, *B, *C ,*D ,*E ,*F, *AB, *ABC, *DE, *DEF;
double promB = 0, promD = 0;
int count_threads = 2;
double timetick;
int N, iter = 1;

struct My_threads {
  int id;
  pthread_t thread;
	double num;
	double *pri;
	double *seg;
	double *res;
};

struct My_threads *p_threads;

int main( int argc, char* argv[] ) {

	//Controla los argumentos al programa
	if (argc < 2) {
		printf("\n Falta un argumento: N dimension de la matriz \n");
		printf("\n 2do argumento opcional: cantidad de hilos \n");
		printf("\n Compilar:\ngcc -pthread -o tp2-ej1-pthreads tp2-ej1-pthreads.c -lm\n");

		return 0;
	}

  N = atoi(argv[1]);

	if (argc >= 3 && atoi(argv[2]) == 4) {
		count_threads = 4;
	}

	if (argc == 4) {
		iter = atoi(argv[3]);
	}

	printf("CANTIDAD DE THREADS: %d.\n", count_threads);

  A = (double*)malloc(sizeof(double)*N*N);
  B = (double*)malloc(sizeof(double)*N*N);
  C = (double*)malloc(sizeof(double)*N*N);
  D = (double*)malloc(sizeof(double)*N*N);
  E = (double*)malloc(sizeof(double)*N*N);
  F = (double*)malloc(sizeof(double)*N*N);
  AB = (double*)calloc(N*N, sizeof(double));
  ABC = (double*)calloc(N*N, sizeof(double));
  DE = (double*)calloc(N*N, sizeof(double));
  DEF = (double*)calloc(N*N, sizeof(double));

  //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      A[i*N+j] = 1;
      B[i+j*N] = 1;
      C[i+j*N] = 1;
      D[i*N+j] = 1;
      E[i+j*N] = 1;
   		F[i+j*N] = 1;
    }
  }

	long double promedio = 0;
	for (int a = 0; a < iter; a++) {

			//arrancan las operaciones y el contador
	  timetick = dwalltime();

	  promB = promedioMatriz(B, N);
	  promD = promedioMatriz(D, N);

	  productoEscalar(promD, A, N);
	  productoEscalar(promB, D, N);

		producto(AB, A, B, N);
	  producto(ABC, AB, C, N);
	  producto(DE, D, E, N);
	  producto(DEF, DE, F, N);
		
		suma(ABC, DEF, N);

		promedio += (dwalltime() - timetick);
	}
	promedio = promedio / iter;
	printf("Tiempo promedio para %d iteracion/es: %LF \n", iter, promedio);

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

void productoEscalar(double x, double* M, int N) {
	p_threads = malloc(sizeof(struct My_threads) * count_threads);

	for (int i = 0; i < count_threads; i++) {
		p_threads[i].id = i;
		p_threads[i].num = x;
		p_threads[i].pri = M;
		pthread_create(&p_threads[i].thread, NULL, pthread_productoEscalar, &p_threads[i].id);
	}

	for (int j = 0; j < count_threads; j++) {
		pthread_join(p_threads[j].thread, NULL);
	}
	free(p_threads);
}

double promedioMatriz(double *M, int n){
	p_threads = malloc(sizeof(struct My_threads) * count_threads);

	void *patial;
	double total = 0.0;

	for (int i = 0; i < count_threads; i++) {
		p_threads[i].id = i;
		p_threads[i].pri = M;
		pthread_create(&p_threads[i].thread, NULL, pthread_promedioMatriz, &p_threads[i].id);
	}

	for (int j = 0; j < count_threads; j++) {
		pthread_join(p_threads[j].thread, NULL);
		total = total + p_threads[j].num;
	}
	free(p_threads);

	return total / (n*n);
}

void producto(double *ret, double *pri, double *seg, int N){
	p_threads = malloc(sizeof(struct My_threads) * count_threads);

	for (int i = 0; i < count_threads; i++) {
		p_threads[i].id = i;
		p_threads[i].pri = pri;
		p_threads[i].seg = seg;
		p_threads[i].res = ret;

		pthread_create(&p_threads[i].thread, NULL, pthread_producto, &p_threads[i].id);
	}

	for (int j = 0; j < count_threads; j++) {
		pthread_join(p_threads[j].thread, NULL);
	}
	free(p_threads);
}


void suma(double *A, double *B, int N) {
	p_threads = malloc(sizeof(struct My_threads) * count_threads);

	for (int i = 0; i < count_threads; i++) {
		p_threads[i].id = i;
		p_threads[i].pri = A;
		p_threads[i].seg = B;
		pthread_create(&p_threads[i].thread, NULL, pthread_suma, &p_threads[i].id);
	}

	for (int j = 0; j < count_threads; j++) {
		pthread_join(p_threads[j].thread, NULL);
	}
	free(p_threads);
}




void * pthread_productoEscalar(void* args){

	int id = *((int *)args);

  int desde = (int)(N*N / count_threads) * id;
  int hasta = (int)(N*N / count_threads) * (id + 1);

	for(int i = desde; i < hasta; i++){
    p_threads[id].pri[i] *= p_threads[id].num;
  }

}

void * pthread_promedioMatriz(void* args){

	int id = *((int *)args);
	int desde = (int)(N*N / count_threads) * id;
  int hasta = (int)(N*N / count_threads) * (id + 1);
	p_threads[id].num = 0;

	for(int i = desde; i < hasta; i++){
		p_threads[id].num += p_threads[id].pri[i];
	}
}

void * pthread_producto(void* args){

	int id = *((int *)args);

	int desde = (int)(N / count_threads) * id;
	int hasta = (int)(N / count_threads) * (id + 1);
	double aux;

	for(int i = desde; i < hasta; i++){
		for(int j = 0; j < N; j++){
			aux = 0;
			for(int k = 0; k < N; k++){
				aux = aux + p_threads[id].pri[i*N + k] * p_threads[id].seg[k + j*N];
			}
			p_threads[id].res[i*N + j] = aux;
	 }
	}
}

void * pthread_suma(void* args){

	int id = *((int *)args);

  int desde = (int)(N*N / count_threads) * id;
  int hasta = (int)(N*N / count_threads) * (id + 1);

	for(int i = desde; i < hasta; i++){
    p_threads[id].pri[i] += p_threads[id].seg[i];
  }
}



/*****************************************************************/

void imprimirMatrizFila(double* A, int N) {
  printf("\n");
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      printf("%d ", (int) A[i*N+j]);
    }
    printf("\n");
  }
  printf("\n");
}

void imprimirMatrizColumna(double* A, int N) {
  printf("\n");
  for(int i=0;i<N;i++){
    	for(int j=0;j<N;j++){
      printf("%d ", (int) A[i+j*N]);
    }
    printf("\n");
  }
  printf("\n");
}

/*****************************************************************/

#include <sys/time.h>

long double dwalltime()
{
	long double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
