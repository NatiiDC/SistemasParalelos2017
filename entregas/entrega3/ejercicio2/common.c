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

int sortFunction(const void* a, const void* b) {
	return ( *((int*)a) - *((int*)b) );
}
