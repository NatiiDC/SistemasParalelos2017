#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int debug;
long iteraciones;
const char* help ="\n./3sec <E>\n<E> = 2^E elementos del vector";

// Data globals.
long long exponent, size;
struct Map {
  int num;
  long occu;
};

struct Map *occurrences;

// --------

int shouldPrint() {
	return debug;
}

double stopwatch() {
	double sec;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

void printArray(int* arr, long long size){
  for (long long i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void printMap(struct Map* map, long long size){
  for (long long i = 0; i < size; i++) {
    printf("(%d:%ld) ", map[i].num, map[i].occu);
  }
  printf("\n");
}

int* newArray(long long size) {
  return malloc(size * sizeof(int));
}

void freeArray(int* array) {
	free(array);
}

int* createArray(long long size){
  int* array = newArray(size);
  time_t t;
  srand((unsigned)time(&t));
  for (size_t i = 0; i < size; i++) {
    array[i] = rand() % size/2;
  }

  return array;
}

int comparison_int(const void *e1, const void *e2) {
    int a = *((int*) e1);
    int b = *((int*) e2);
    if (a < b) {
        return -1;
    } else if (a > b) {
        return 1;
    } else {
        return 0;
    }
}

void sort(int* arr) {
    qsort(arr, size, sizeof(int), comparison_int);
}

int comparison_map(const void *e1, const void *e2) {
    struct Map a = *((struct Map*) e1);
    struct Map b = *((struct Map*) e2);
    if (a.occu > b.occu) {
        return -1;
    } else if (a.occu < b.occu) {
        return 1;
    } else {
        return 0;
    }
}

struct Map* pairsOfOccurrences(int *arrSort) {
  int num = arrSort[0];
  long occurrences = 0;
  iteraciones = 0;

  long sizeAlloc = 1;
  int temp = num;
  for (long long i = 0; i < size; i++) {
    if (temp != arrSort[i]){
      sizeAlloc++;
      temp = arrSort[i];
    }
  }

  struct Map *map = malloc(sizeof(struct Map)*sizeAlloc);

  for (long long i = 0; i < size; i++) {
    if (num != arrSort[i]) {
      map[iteraciones].num = num;
      map[iteraciones].occu = occurrences;

      occurrences = 1;
      num = arrSort[i];
      iteraciones++;
    } else {
      occurrences++;
    }
  }
  map[iteraciones].num = num;
  map[iteraciones].occu = occurrences;

  qsort(map, iteraciones, sizeof(struct Map), comparison_map);
  return map;
}



int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("%s\n", help);
    return 0;
  }

  /* Data Globals initialization.*/
	exponent = atoi(argv[1]);
	size = pow(2, exponent);
	debug = 0;
	if (argc > 2) {
		debug = 1;
	}

	if (shouldPrint()) {
		printf("exponent: %lld\nsize: %lld\n", exponent, size);
	}

  double start;
	start = stopwatch();

  int* arr = createArray(size);
  sort(arr);
  struct Map *pairsSort = pairsOfOccurrences(arr);

  int sizeArrOccu = (iteraciones <= 100)?iteraciones:100;
  int mostOccurrences[sizeArrOccu];
  for (int i = 0; i < sizeArrOccu; i++) {
    mostOccurrences[i] = pairsSort[i].num;
  }
  double end = stopwatch();
  double delta = end - start;
  printf("total seconds: %f\n", delta);

  if (shouldPrint()){
    printf("Cantidad de pares: %d\n", sizeArrOccu);
    printArray(mostOccurrences, sizeArrOccu);
  }

  return 0;
}
