#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <signal.h>

#define MAX_LENGTH 1025


void exitMsg(int exitCode, char* message) {
	printf("%s\n", message);
	exit(exitCode);
}

void signalHandler(int signum, siginfo_t* info, void* pointer) {
	printf("Signal sent from process %lu\n", (unsigned long) info->si_pid);
}

int getNumberOfProcesses(ssize_t fileSize) {
	int numOfCounterProcesses = 0;
	int pageSize = getpagesize();
	int chunkSize = pageSize;

	if (pageSize <= 0) { //shouldn't happen, but just in case.
		exitMsg(EXIT_FAILURE, "Error: Cannot get page size\n");
	}

	//compute the number of counter processes needed
	if (fileSize < 2 * pageSize) {
		numOfCounterProcesses = 1;
	} else {
		//calculate chunk size
		while ((fileSize / chunkSize) > 16) {
			chunkSize += pageSize;
		}

		numOfCounterProcesses = fileSize / chunkSize;
	}

	//TODO - delete
	printf("Number of processes %d\n", numOfCounterProcesses);
	printf("Chunk size: %d\n", chunkSize);

	return numOfCounterProcesses;
}


int main(int argc, char** argv) {
	char charToCount;
	char filePath[MAX_LENGTH];
	char errorMsg[MAX_LENGTH];
	int totalCharCount = 0;
	int pageSize = 0;
	int numOfCounterProcesses = 0;
	ssize_t chuckSize = 0;
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

	//open file
	int fd = open(filePath, O_RDONLY);
	if (fd < 0) {
		sscanf(errorMsg, "Error: Cannot move file cursor: %s", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	//get stats for file size
	if (fstat(fd, &status) < 0) {
		sscanf(errorMsg, "Error: Cannot get file stats: %s", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	numOfCounterProcesses = getNumberOfProcesses(status.st_size);

	memset(&new_action, 0, sizeof(new_action));
	new_action.sa_handler = signalHandler;
	new_action.sa_flags = SA_SIGINFO;

	if ((sigaction(SIGUSR1, &new_action, NULL)) != 0) {
		printf("Signal handle registration failed. %s\n", strerror(errno));
		return -1;
	}

	//for each proccess until all of them are finished

	//open named pipe file called “/tmp/counter_PID” for reading
	//read current counter result from the pipe
	//aggregate results to a global data structure

	printf("The character %c appears %d times in the file\n", charToCount, totalCharCount);
	return 0;
}

