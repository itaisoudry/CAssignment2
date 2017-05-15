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
#include <unistd.h>

#define MAX_LENGTH 1025
#define COUNTER_PATH "./counter.c"
#define PIPE_NAME "/tmp/counter_"

void exitMsg(int exitCode, char* message) {
	printf("%s\n", message);
	exit(exitCode);
}
size_t charCount = 0;
void signalHandler(int signum, siginfo_t* info, void* ptr) {
	char pipeName[MAX_LENGTH];
	char errorMsg[MAX_LENGTH];
	char buffer[MAX_LENGTH];
	unsigned long pid = (unsigned long) info->si_pid;
	printf("Signal sent from process %lu\n", pid);

	kill(pid, SIGUSR2);
	//read from pipe
	strcpy(pipeName, PIPE_NAME);
	strcat(pipeName, pid);

	int fd = open(pipeName, O_RDONLY);
	if (fd == -1) {
		sscanf(errorMsg, "Error: Cannot open file %s: %s\n", pipeName, strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	if (read(fd, buffer, MAX_LENGTH) == -1) {
		sscanf(errorMsg, "Error: Reading from file %s failed: %s\n", pipeName, strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	charCount += atoi(buffer);

	if (close(fd) == -1) {
		sscanf(errorMsg, "Error: Closing file %s failed: %s\n", pipeName, strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

}

pid_t forkCounter(char charToCount, char* filePath, size_t offSet, size_t size) {
	pid_t pid;
	char* args[5];

	// read current counter result from the pipe
	if ((pid = fork()) == -1) {
		printf("Fork failed. %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	if (pid == 0) { //child
		strcpy(args[0], COUNTER_PATH);
		strcpy(args[1], charToCount);
		strcpy(args[2], filePath);
		sscanf(args[3], "%zu", offSet);
		sscanf(args[4], "%zu", size);

		execv(COUNTER_PATH, args);
	}

	return pid;
}

int getNumberOfProcesses(size_t fileSize) {
	int numOfCounterProcesses = 0;
	int pageSize = getpagesize();
	int chunkSize = pageSize;

	if (pageSize <= 0) {  // shouldn't happen, but just in case.
		exitMsg(EXIT_FAILURE, "Error: Cannot get page size\n");
	}

// compute the number of counter processes needed
	if (fileSize < 2 * pageSize) {
		numOfCounterProcesses = 1;
	} else {
		// calculate chunk size
		while ((fileSize / chunkSize) > 16) {
			chunkSize += pageSize;
		}

		numOfCounterProcesses = fileSize / chunkSize;
	}

// TODO - delete
	printf("Number of processes %d\n", numOfCounterProcesses);
	printf("Chunk size: %d\n", chunkSize);

	return numOfCounterProcesses;
}

int main(int argc, char** argv) {
	char charToCount;
	char filePath[MAX_LENGTH];
	char errorMsg[MAX_LENGTH];
	int pageSize = 0;
	int numOfCounterProcesses = 0;
	int pipeFd[2];
	size_t chuckSize = 0;
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
		sscanf(errorMsg, "Error: Cannot move file cursor: %s", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	// get stats for file size
	if (fstat(fd, &status) < 0) {
		sscanf(errorMsg, "Error: Cannot get file stats: %s", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	if (status.st_size <= 0) {
		exitMsg(EXIT_FAILURE, "Error: file is empty\n");

	}
	numOfCounterProcesses = getNumberOfProcesses(status.st_size);

	memset(&new_action, 0, sizeof(new_action));
	new_action.sa_sigaction = signalHandler;
	new_action.sa_flags = SA_SIGINFO;

	if ((sigaction(SIGUSR1, &new_action, NULL)) != 0) {
		printf("Signal handle registration failed. %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

// for each process until all of them are finished
	for (int i = 0; i < numOfCounterProcesses; i++) {

		if (pipe(pipeFd) == -1) {
			printf("Signal handle registration failed. %s\n", strerror(errno));
			return EXIT_FAILURE;
		}

		pid = forkCounter(charToCount, filePath, 0, 0);
	}

	wait(NULL);

	printf("The character %c appears %zu times in the file\n", charToCount, charCount);

	return 0;
}
