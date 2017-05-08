#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main( int argc, char* argv[] ) {

  int total = 0;
  omp_set_num_threads(4);

  #pragma omp parallel for reduction(+: total)
	for (int i = 0; i < 100; i++){
		total += 1;
	}
  printf("Total: %d\n", total);

}
