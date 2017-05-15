/*
 * counter.c
 *
 *  Created on: May 10, 2017
 *      Author: soudry
 */

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

#define PIPE_NAME "/tmp/counter_"
#define MAX_LENGTH 1024
#define BUFFER_SIZE 4097
size_t count = 0;

void exitMsg(int exitCode, char* message) {
	printf("%s\n", message);
	exit(exitCode);
}

int countChar(char charToCount, char buffer[], int bufferSize) {
	int count = 0;

	for (int i = 0; i < bufferSize; i++) {
		if (buffer[i] == charToCount)
			count++;
	}

	return count;
}

void signalHandler(int signum, siginfo_t* info, void* ptr) {
	char pipeName[MAX_LENGTH];
	char errorMsg[MAX_LENGTH];
	char countStr[MAX_LENGTH];
	int fd = 0;

	if ((fd = open(pipeName, O_WRONLY)) == -1) {
		sscanf(errorMsg, "Error: Cannot open file %s: %s\n", pipeName, strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	sscanf(countStr, "%zu", &count);

	if (write(fd, countStr, sizeof(count)) < 0) {
		sscanf(errorMsg, "Error: write to file %s failed: %s\n", pipeName, strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	sleep(1);

	close(fd);
	remove(pipeName);

}

int main(int argc, char** argv) {
	char filePath[MAX_LENGTH];
	char errorMsg[MAX_LENGTH];
	char charToCount = 0;
	char pipeName[MAX_LENGTH];
	char pidStr[MAX_LENGTH];
	off_t offSet = 0;
	size_t length = 0;
	int fd;
	char* arr;
	pid_t pid = 0;
	struct sigaction new_action;

	if (argc != 5) {
		exitMsg(EXIT_FAILURE, "Error: Invalid arguments\n");
	}
	printf("THIS IS COUNTER\n");
	charToCount = argv[1][0];
	strcpy(filePath, argv[2]);
	sscanf(argv[3],"%lu",&offSet);
	sscanf(argv[4],"%zu",&length);

	//set handler
	memset(&new_action, 0, sizeof(new_action));
	new_action.sa_sigaction = signalHandler;
	new_action.sa_flags = SA_SIGINFO;

	//register signal handler
	if ((sigaction(SIGUSR2, &new_action, NULL)) != 0) {
		printf("Signal handle registration failed. %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	// open file
	fd = open(filePath, O_RDONLY);
	if (fd < 0) {
		sscanf(errorMsg, "Error: Cannot open file: %s", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	printf("filePath=%s , offset=%lu, length=%zu\n",filePath,offSet,length);
	printf("%d\n",fd);

	arr = (char*) mmap(NULL,length, PROT_READ, MAP_SHARED, fd, 0);
	if (arr == MAP_FAILED) {
		sscanf(errorMsg, "Error mmapping the file: %s\n", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}



	// read and count characters
	for (int i = 0; i < length; i++)
		if (arr[i] == charToCount)
			count++;

	//printf("%d\n",count);

	if (munmap(arr, length) == -1) {
		sscanf(errorMsg, "Error un-mmapping the file: %s\n", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}
	//finished processing number of characters
	close(fd);

	// determine process id
	pid = getpid();
	// create named pipe file named “/tmp/counter_PID” and open for writing
	strcpy(pipeName, PIPE_NAME);
	sscanf(pidStr, "%d", &pid);
	strcat(pipeName, pidStr);

	printf("OK2\n");
	if (mkfifo(pipeName, 0644) == -1) {
		sscanf(errorMsg, "Error: mkfifo failed: %s\n", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}



	while (1) {
		kill(getppid(), SIGUSR1);
	}

	return 0;
}

