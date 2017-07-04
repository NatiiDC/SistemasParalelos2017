// Worker functions.
#include <limits.h>

typedef struct {
	int e;
	int index;
} tMin;

typedef struct {
	int* base;
	int* it;
	int* end;
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

void setupWorker() {
	mergedArray.maxArrays = 32;
	mergedArray.arrayCount = 0;
	mergedArray.sortedArrays = malloc(sizeof(tArray*) * mergedArray.maxArrays);
}

void freeTArray(tArray* array) {
	free(array->base);
}

void cleanupWorker() {
	for (long long i = 0; i < mergedArray.arrayCount; i++) {
		freeTArray(mergedArray.sortedArrays[i]);
	}
	free(mergedArray.sortedArrays);
	free(mergedArray.base);
}

void saveArray(int* array) {
	if (mergedArray.arrayCount >= mergedArray.maxArrays) {
		mergedArray.maxArrays *= 2;
		mergedArray.sortedArrays = realloc(mergedArray.sortedArrays, mergedArray.maxArrays);
	}
	tArray* sortedArray = &(mergedArray.sortedArrays[mergedArray.arrayCount++]);
	sortedArray->base = array;
	sortedArray->it = array;
	sortedArray->end = array + scatteredSize;
}

void sort() {
	int* array = receiveArray(MASTER, scatteredSize);
	qsort(array, scatteredSize, sizeof(int), sortFunction);
	saveArray(array);
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

void initMultiMerge() {
	mergedArray.size = scatteredSize * mergedArray.arrayCount;
	mergedArray.base = malloc(sizeof(int) * mergedArray.size);
	mergedArray.it = mergedArray.base;
}

void merge() {
	initMultiMerge();
	do {
		saveMin(findMin());
	} while(!endMultiMerge());
}

void sendMerged() {
	sendSize(MASTER, mergedArray.size);
	sendArray(MASTER, mergedArray.base, mergedArray.size);
}

int getLockFor(int task) {
	MPI_Send(&task, 1, MPI_INT, MASTER, T_LOCK, WORLD);
}

int getTask() {
	int result;
	MPI_Status status;
	getLockFor(NONE);
	MPI_Recv(&result, 1, MPI_INT, MASTER, T_TASK, WORLD, &status);
	return result;
}

void executeTask(int task) {
	printf("WORKER %d got task %s.\n", id, printTask(task));
	switch (task) {
		case SORT:
		{
			sort();
			break;
		}
		case MERGE:
		{
			merge();
			getLockFor(MERGE);
			sendMerged();
			getLockFor(EXIT);
			break;
		}
		case NONE:
		case EXIT:
		default:
		{
			break;
		}
	}
}

void worker() {
	printf("WORKER %d started to run.\n", id);
	setupWorker();
	int currentTask = NONE;
	do {
		currentTask = getTask();
		executeTask(currentTask);
	} while (currentTask != EXIT);
	cleanupWorker();
}
