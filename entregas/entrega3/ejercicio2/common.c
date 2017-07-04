// Constant definitions.
#ifndef MASTER
#define MASTER 0
#endif
#ifndef WORLD
#define WORLD MPI_COMM_WORLD
#endif

#ifndef T_TASK
#define T_TASK 1
#endif
#ifndef T_SIZE
#define T_SIZE 2
#endif
#ifndef T_ARRAY
#define T_ARRAY 3
#endif
#ifndef T_LOCK
#define T_LOCK 4
#endif

#ifndef EXIT
#define EXIT 0
#endif
#ifndef NONE
#define NONE 1
#endif
#ifndef SORT
#define SORT 2
#endif
#ifndef MERGE
#define MERGE 3
#endif

int isMaster() {
	return id == MASTER;
}

// Debug functions.
int shouldPrint() {
	return debug && isMaster();
}

const char* printTask(int task) {
	switch (task) {
		case EXIT:
			return "EXIT";
		case NONE:
			return "NONE";
		case SORT:
			return "SORT";
		case MERGE:
			return "MERGE";
	}
}

void printArray(int* array) {
	for (long long i = 0; i < size; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

void printScatteredArray(int* array) {
	for (long long i = 0; i < scatteredSize; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

void printSizedArray(int* array, long long size) {
	for (long long i = 0; i < size; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

double stopwatch() {
	double sec;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

// Array functions.
int* newArray(long long size) {
	return malloc(size * sizeof(int));
}

void freeArray(int* array) {
	free(array);
}

int* initArray() {
	int* array = newArray(size);
	time_t t;
	srand((unsigned) time(&t));
	for (long long i = 0; i < size; i++) {
		array[i] = rand() % 1000;
	}
	return array;
}

int sortFunction(const void* a, const void* b) {
	return ( *((int*)a) - *((int*)b) );
}

// Multimerge functions
#include <limits.h>
#include <string.h>

typedef struct {
	int e;
	int index;
} tMin;

typedef struct {
	int* base;
	int* it;
	int* end;
	long long size;
} tArray;

typedef struct {
	tArray* sortedArrays;
	long long maxArrays;
	long long arrayCount;
	int* base;
	int* it;
	int* end;
	long long size;
} tMerged;

tMerged mergedArray;

void saveArray(int* array, long long size) {
	if (mergedArray.arrayCount >= mergedArray.maxArrays) {
		mergedArray.maxArrays *= 2;
		mergedArray.sortedArrays = realloc(mergedArray.sortedArrays, mergedArray.maxArrays);
	}
	tArray* sortedArray = &(mergedArray.sortedArrays[mergedArray.arrayCount++]);
	sortedArray->base = array;
	sortedArray->it = array;
	sortedArray->end = array + size;
	sortedArray->size = size;
}

void freeTArray(tArray* array) {
	freeArray(array->base);
}

void setupMergeArray() {
	mergedArray.maxArrays = 32;
	mergedArray.arrayCount = 0;
	mergedArray.sortedArrays = malloc(sizeof(tArray*) * mergedArray.maxArrays);
}

void initMultiMerge() {
	long long size = 0;
	for (long long i = 0; i < mergedArray.arrayCount; i++) {
		size += mergedArray.sortedArrays[i].size;
	}
	mergedArray.base = newArray(mergedArray.size);
	mergedArray.it = mergedArray.base;
	mergedArray.end = mergedArray.base + size;
	mergedArray.size = size;
}

void cleanupMergeArray() {
	for (long long i = 0; i < mergedArray.arrayCount; i++) {
		freeTArray(&(mergedArray.sortedArrays[i]));
	}
	freeArray(mergedArray.base);
	free(mergedArray.sortedArrays);
}

tMin findMin() {
	tMin min = {INT_MAX, -1};
	for (long long i = 0; i < mergedArray.arrayCount; i++) {
		tArray* array = &mergedArray.sortedArrays[i];
		if (array->it < array->end && *array->it <= min.e) {
			min.e = *array->it;
			min.index = i;
		}
	}
	return min;
}

void saveMin(tMin min) {
	if (min.index != -1) {
		*mergedArray.it = min.e;
		mergedArray.sortedArrays[min.index].it++;
	}
	mergedArray.it++;
}

void merge() {
	initMultiMerge();
	//if only one array copy.
	if (mergedArray.arrayCount == 1) {
		memcpy(mergedArray.base, mergedArray.sortedArrays[0].base, mergedArray.size);
		return;
	}
	//else merge
	do {
		saveMin(findMin());
	} while(mergedArray.it < mergedArray.end);
}
