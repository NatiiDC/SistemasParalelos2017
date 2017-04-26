//Ejercicio 1
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc,char*argv[]) {
  double *A;
  int i,j;
  int check = 1;

  if (argc < 3) {
    printf("\n Falta dos argumentos: \n");
    printf("* dimension de la matriz \n");
    printf("* cantidad de threads \n");
    printf("Compilar:\ngcc -fopenmp -o ejercicio1 ejericio1.c\n");
    printf("Ejecutar:\n./ejercicio1 N cantidadThreads.c\n");
    return 0;
  }

  int numThreads = atoi(argv[2]);
  int N = atoi(argv[1]);
  omp_set_num_threads(numThreads);

  A = (double*)malloc(sizeof(double)*N*N);

 //Inicializa la matriz. Cada posicion debe quedar con el valor I*J
 // I => fila J=> columna.

  for (i = 0; i < N; i++) {
    #pragma omp parallel for shared(A)
      for(j=0;j<N;j++) {
        A[i*N+j]=i*j;
      }
  }

  //Verifica el resultado
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      check = check && (A[i*N+j]==i*j);
    }
  }

  if (check) {
   printf("Resultado correcto\n");
  }else{
   printf("Resultado erroneo \n");
  }

 free(A);

 return(0);
}
