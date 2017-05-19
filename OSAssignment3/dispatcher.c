#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LENGTH 1025
#define COUNTER_PATH "./counter"
#define PIPE_NAME "/tmp/counter_"

void exitMsg(int exitCode, char* message) {
	printf("%s\n", message);
	exit(exitCode);
}
size_t charCount = 0;
int signalsCount = 0;
void signalHandler(int signum, siginfo_t* info, void* ptr) {
	char pipeName[MAX_LENGTH];
	char errorMsg[MAX_LENGTH];
	char buffer[MAX_LENGTH];
	char pidStr[MAX_LENGTH];
	unsigned long pid = info->si_pid;

	printf("Signal sent from process %lu\n", pid);
	signalsCount++;
	//create pipe file name
	strcpy(pipeName, PIPE_NAME);
	sprintf(pidStr, "%lu", pid);
	strcat(pipeName, pidStr);

	int fd = open(pipeName, O_RDONLY);
	if (fd == -1) {
		sprintf(errorMsg, "Error: Cannot open file %s: %s\n", pipeName, strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	if (read(fd, buffer, MAX_LENGTH) == -1) {
		sprintf(errorMsg, "Error: Reading from file %s failed: %s\n", pipeName, strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	charCount += atoi(buffer);

	if (close(fd) == -1) {
		sprintf(errorMsg, "Error: Closing file %s failed: %s\n", pipeName, strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

}

pid_t forkCounter(char charToCount, char* filePath, size_t offSet, size_t size) {
	pid_t pid;
	char* args[6];
	char sizeStr[MAX_LENGTH];
	char offsetStr[MAX_LENGTH];

	// read current counter result from the pipe
	if ((pid = fork()) == -1) {
		printf("Fork failed. %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	if (pid == 0) { //child
		args[0] = COUNTER_PATH;
		args[1] = &charToCount;
		args[2] = filePath;

		sprintf(offsetStr, "%zu", offSet);
		args[3] = offsetStr;

		sprintf(sizeStr, "%zu", size);
		args[4] = sizeStr;
		args[5] = NULL;

		execv(COUNTER_PATH, args);
	}

	return pid;
}

int getNumberOfProcesses(size_t fileSize, size_t* chunkSize) {
	int numOfCounterProcesses = 0;
	int pageSize = getpagesize();
	*chunkSize = pageSize;

	if (pageSize <= 0) {  // shouldn't happen, but just in case.
		exitMsg(EXIT_FAILURE, "Error: Cannot get page size\n");
	}

// compute the number of counter processes needed
	if (fileSize < 2 * pageSize) {
		numOfCounterProcesses = 1;
	} else {
		// calculate chunk size
		while (((((double) fileSize) / (*chunkSize))) > 16.0) {
			*chunkSize += pageSize;
		}
		numOfCounterProcesses = (fileSize / (*chunkSize)) + 1;

		if (numOfCounterProcesses > 16)
			numOfCounterProcesses = 16;  //just to be sure
	}

	printf("Creating %d processes, file size: %zu, chunk size %lu\n", numOfCounterProcesses, fileSize, *chunkSize);
	return numOfCounterProcesses;
}

int main(int argc, char** argv) {
	char charToCount;
	char filePath[MAX_LENGTH];
	char errorMsg[MAX_LENGTH];
	int pageSize = 0;
	int numOfCounterProcesses = 0;
	int pipeFd[2];
	size_t totalSize = 0;
	size_t chunkSize = 0;
	pid_t pid;
	struct stat status;
	struct sigaction new_action;

	if (argc != 3) {
		exitMsg(EXIT_FAILURE, "Error: Invalid arguments");
	}

	if (strlen(argv[1]) != 1 || !isalpha(argv[1][0])) {
		exitMsg(EXIT_FAILURE, "Error: Input should be exactly one character");
	}

	charToCount = argv[1][0];
	strcpy(filePath, argv[2]);

	// open file
	int fd = open(filePath, O_RDONLY);
	if (fd < 0) {
		sprintf(errorMsg, "Error: Cannot move file cursor: %s", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	// get stats for file size
	if (fstat(fd, &status) < 0) {
		sprintf(errorMsg, "Error: Cannot get file stats: %s", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	if (status.st_size <= 0) {
		exitMsg(EXIT_FAILURE, "Error: file is empty\n");

	}
	numOfCounterProcesses = getNumberOfProcesses(status.st_size, &chunkSize);

	memset(&new_action, 0, sizeof(new_action));
	new_action.sa_sigaction = signalHandler;
	new_action.sa_flags = SA_SIGINFO;

	if ((sigaction(SIGUSR1, &new_action, NULL)) != 0) {
		printf("Signal handle registration failed. %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	totalSize = status.st_size;
	size_t originalChunk = chunkSize;
	int activeProcessCount = 0;
// for each process until all of them are finished
	for (int i = 0; i < numOfCounterProcesses && totalSize > 0; i++) {

		if (totalSize < chunkSize) {
			printf("Changed total %lu and chunk:%lu\n", totalSize,  chunkSize);
			chunkSize = totalSize;
			printf("New Chunk: %lu",chunkSize);

		}

		if (pipe(pipeFd) == -1) {
			printf("Signal handle registration failed. %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
		pid = forkCounter(charToCount, filePath, (originalChunk * i), chunkSize);

		totalSize -= chunkSize;
		activeProcessCount++;
		printf("Fork: %d, total left: %lu\n", pid, totalSize);
	}

	printf("active process count: %d\n", activeProcessCount);
	int st;
	for (int i = 0; i < activeProcessCount; i++)
		while (-1 == waitpid(-1, &st, 0))
			;

	printf("The character %c appears %zu times in the file\n", charToCount, charCount);
	printf("Num of signals returned: %d\n", signalsCount);
	return 0;
}
