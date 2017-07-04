//Master Functions.

typedef struct {
	int* base;
	int* it;
} tUnordered;

tUnordered uArray;

void setupMaster() {
	uArray.base = initArray();
	uArray.it = uArray.base - scatteredSize;
	setupMergeArray();
}

void cleanupMaster() {
	freeArray(uArray.base);
	cleanupMergeArray();
}

int* nextArray() {
	uArray.it += scatteredSize;
	return (uArray.it < (uArray.base + size))? uArray.it : NULL;
}

int processRequest(int task, int sender) {
	switch (task) {
		// Asking for work.
		case NONE:
		{
			if (nextArray() != NULL) {
				sendSort(sender, uArray.it);
			} else {
				sendMerge(sender);
			}
			return 0;
		}
		// Receiving sorted/ merged arrays.
		case SORT:
		case MERGE:
		{
			long long size = receiveSize(sender);
			int* array = receiveArray(sender, size);
			saveArray(array, size);
			return 0;
		}
		// Abort.
		case EXIT:
		default:
		{
			sendTask(sender, EXIT);
			return 1;
		}
	}
}

int waitForWorker() {
	int task;
	MPI_Status status;
	MPI_Recv(&task, 1, MPI_INT, MPI_ANY_SOURCE, T_LOCK, WORLD, &status);
	return processRequest(task, status.MPI_SOURCE);
}

void master() {
	setupMaster();
	//printf("MASTER started to run.\n");
	int doneWorkers = 0;
	do {
		doneWorkers += waitForWorker();
	} while(doneWorkers < (paralelism - 1));
	//merge();
	//printArray(mergedArray.base);
	//cleanupMaster();
}
