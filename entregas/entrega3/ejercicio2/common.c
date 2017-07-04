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

int* createArray() {
	if (isMaster()) {
		return initArray();
	}
	return newArray(scatteredSize);
}

int sortFunction(const void* a, const void* b) {
	return ( *((int*)a) - *((int*)b) );
}

// Comunication Functions
void sendSize(int receiver, long long size) {
	MPI_Send(&size, 1, MPI_LONG_LONG_INT, receiver, T_SIZE, WORLD);
}

long long receiveSize(int sender) {
	long long size;
	MPI_Status status;
	MPI_Recv(&size, 1, MPI_LONG_LONG_INT, sender, T_SIZE, WORLD, &status);
	return size;
}

void sendArray(int receiver, int* array, long long size) {
	MPI_Send(array, size, MPI_INT, receiver, T_ARRAY, WORLD);
}

int* receiveArray(int sender, long long size) {
	int* array = newArray(size);
	MPI_Status status;
	MPI_Recv(array, size, MPI_INT, sender, T_ARRAY, WORLD, &status);
	return array;
}

int getLockFor(int task) {
	MPI_Send(&task, 1, MPI_INT, MASTER, T_LOCK, WORLD);
}

void sendTask(int receiver, int task) {
	MPI_Send(&task, 1, MPI_INT, receiver, T_TASK, WORLD);
}

int getTask() {
	int result;
	MPI_Status status;
	getLockFor(NONE);
	MPI_Recv(&result, 1, MPI_INT, MASTER, T_TASK, WORLD, &status);
	return result;
}

void sendSort(int receiver, int* array) {
	sendTask(receiver, SORT);
	sendArray(receiver, array, scatteredSize);
}

void sendMerge(int receiver) {
	sendTask(receiver, MERGE);
}


// Multimerge functions
#include <limits.h>

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
	long long maxArrays;
	long long arrayCount;
	long long size;
	tArray* sortedArrays;
	int* base;
	int* it;
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

void freeTArray(tArray array) {
	free(array.base);
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
	mergedArray.size = size;
	mergedArray.base = malloc(sizeof(int) * mergedArray.size);
	mergedArray.it = mergedArray.base;
}

void cleanupMergeArray() {
	for (long long i = 0; i < mergedArray.arrayCount; i++) {
		freeTArray(mergedArray.sortedArrays[i]);
	}
	free(mergedArray.sortedArrays);
	free(mergedArray.base);
}

int endMultiMerge() {
	for (long long i = 0; i < mergedArray.arrayCount; i++) {
		tArray* array = &mergedArray.sortedArrays[i];
		if(array->it != array->end) return 0;
	}
	return 1;
}

tMin findMin() {
	tMin min = {INT_MAX, -1};
	for (long long i = 0; i < mergedArray.arrayCount; i++) {
		tArray* array = &mergedArray.sortedArrays[i];
		if (array->it != array->end) {
			if (*array->it <= min.e) {
				min.e = *array->it;
				min.index = i;
			}
		}
	}
	mergedArray.sortedArrays[min.index].it++;
	return min;
}

void saveMin(tMin min) {
	*mergedArray.it = min.e;
	mergedArray.it++;
}

void merge() {
	initMultiMerge();
	do {
		saveMin(findMin());
	} while(!endMultiMerge());
}
