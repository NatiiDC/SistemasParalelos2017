// Worker functions.
void setupWorker() {
	setupMergeArray();
}

void cleanupWorker() {
	cleanupMergeArray();
}

void sort() {
	int* array = receiveArray(MASTER, scatteredSize);
	qsort(array, scatteredSize, sizeof(int), sortFunction);
	saveArray(array, scatteredSize);
}

void sendMerged() {
	sendSize(MASTER, mergedArray.size);
	sendArray(MASTER, mergedArray.base, mergedArray.size);
}

void executeTask(int task) {
	//printf("WORKER %d got task %s.\n", id, printTask(task));
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
	//printf("WORKER %d started to run.\n", id);
	setupWorker();
	int currentTask = NONE;
	do {
		currentTask = getTask();
		executeTask(currentTask);
	} while (currentTask != EXIT);
	//cleanupWorker();
}
