// Comunication Functions
void sendSize(int receiver, long long size) {
	MPI_Ssend(&size, 1, MPI_LONG_LONG_INT, receiver, T_SIZE, WORLD);
}

long long receiveSize(int sender) {
	long long size;
	MPI_Status status;
	MPI_Recv(&size, 1, MPI_LONG_LONG_INT, sender, T_SIZE, WORLD, &status);
	printf("MERGED SIZE %lld\n", size);
	return size;
}

void sendArray(int receiver, int* array, long long size) {
	MPI_Ssend(array, size, MPI_INT, receiver, T_ARRAY, WORLD);
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

int getTask() {
	int result;
	MPI_Status status;
	getLockFor(NONE);
	MPI_Recv(&result, 1, MPI_INT, MASTER, T_TASK, WORLD, &status);
	return result;
}

void sendTask(int receiver, int task) {
	MPI_Send(&task, 1, MPI_INT, receiver, T_TASK, WORLD);
}

void sendSort(int receiver, int* array) {
	sendTask(receiver, SORT);
	sendArray(receiver, array, scatteredSize);
}

void sendMerge(int receiver) {
	sendTask(receiver, MERGE);
}
