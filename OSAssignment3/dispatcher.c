/*
 * dispatcher.c
 *
 *  Created on: May 10, 2017
 *      Author: soudry
 */
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

#define MAX_LENGTH 1025

int main1(int argc, char** argv) {
	char charToCount;
	char filePath[MAX_LENGTH];
	char errorMsg[MAX_LENGTH];
	int numOfCounterProccesses = 0;
	int count = 0;

	struct stat status;

	if (argc != 2) {
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

	//compute 'M' the number of counter proccesses needed
	numOfCounterProccesses = sqrt(status.st_size);

	//M*(M ± 2) Counter processes
	numOfCounterProccesses = numOfCounterProccesses*(numOfCounterProccesses-2);

	//for each proccess until all of them are finished

	//open named pipe file called “/tmp/counter_PID” for reading
	//read current counter result from the pipe
	//aggregate results to a global data structure

	printf("The character %c appears %d times in the file\n",charToCount,count);
	return 0;
}

void exitMsg1(int exitCode, char* message) {
	printf("%s\n", message);
	exit(exitCode);
}
