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

int main(int argc, char** argv) {
	char filePath[MAX_LENGTH];
	char errorMsg[MAX_LENGTH];
	char charToCount = 0;
	char pipeName[MAX_LENGTH];
	char pidStr[MAX_LENGTH];
	char resultStr[MAX_LENGTH];
	off_t offSet = 0;
	size_t length = 0;
	int fd;
	char* arr;
	pid_t pid = 0;
	struct sigaction new_action;

	if (argc != 5) {
		exitMsg(EXIT_FAILURE, "Error: Invalid arguments\n");
	}
	charToCount = argv[1][0];
	strcpy(filePath, argv[2]);
	sscanf(argv[3], "%lu", &offSet);
	sscanf(argv[4], "%zu", &length);

	// open file
	fd = open(filePath, O_RDONLY);
	if (fd < 0) {
		sprintf(errorMsg, "Error: Cannot open file: %s", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	//printf("filePath=%s , offset=%lu, length=%zu , pid=%d\n", filePath, offSet, length,getpid());
//	printf("%d\n", fd);

	arr = (char*) mmap(NULL, (size_t)length, PROT_READ, MAP_SHARED, fd, offSet);
	if (arr == MAP_FAILED) {
		sprintf(errorMsg, "Error mmapping the file: %s\n", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	// read and count characters
	for (int i = 0; i < length; i++)
		if (arr[i] == charToCount)
			count++;

	close(fd);

	// determine process id
	pid = getpid();
	if (count > 0)
		printf("Counted %lu in %d\n", count, pid);

	// create named pipe file named “/tmp/counter_PID” and open for writing
	strcpy(pipeName, PIPE_NAME);
	sprintf(pidStr, "%d", pid);
	strcat(pipeName, pidStr);

	//printf("Creating pipe: %s\n", pipeName);

	if (mkfifo(pipeName, 0666) == -1) {
		sprintf(errorMsg, "Error: mkfifo failed: %s\n", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	sleep(pid % 16);

	if (kill(getppid(), SIGUSR1) == -1) {
		sprintf(errorMsg, "Error: Cannot send signal: %s\n", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}
	printf("Signal sent pid=%d\n",pid);

	int pipeFd = open(pipeName, O_WRONLY);
	if (pipeFd == -1) {
		sprintf(errorMsg, "Error: Cannot open file %s: %s", pipeName, strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	sprintf(resultStr, "%zu", count);

	if (write(pipeFd, resultStr, strlen(resultStr)) == -1) {
		sprintf(errorMsg, "Error: Cannot write to file %s: %s", pipeName, strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}


	//unmap and close after writing
	if (munmap(arr, length) == -1) {
		sprintf(errorMsg, "Error un-mmapping the file: %s\n", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	close(pipeFd);
	remove(pipeName);

	return 0;
}

