/*
 * counter.c
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
#include <stdbool.h>

#define MAX_LENGTH 1025
#define BUFFER_SIZE 4097

int countChar(char charToCount, char buffer[], int bufferSize);
void exitMsg(int exitCode, char* message);

int main(int argc, char** argv) {
	char filePath[MAX_LENGTH];
	char errorMsg[MAX_LENGTH];
	char buffer[BUFFER_SIZE];
	char charToCount = 0;
	int offSet = 0;
	int length = 0;
	int count = 0;
	ssize_t totalBytesToRead = 0;
	int bytesToRead = BUFFER_SIZE;
	int bytesRead = 0;

	if (argc != 4) {
		exitMsg(EXIT_FAILURE, "Error: Invalid arguments");
	}

	charToCount = argv[1][0];
	strcpy(filePath, argv[2]);
	offSet = atoi(argv[3]);
	length = atoi(argv[4]);

	//open file
	int fd = open(filePath, O_RDONLY);
	if (fd < 0) {
		sscanf(errorMsg, "Error: Cannot open file: %s", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	//move cursor to offset
	if (lseek(fd, offSet, SEEK_SET) < 0) {
		sscanf(errorMsg, "Error: Cannot move file cursor: %s", strerror(errno));
		exitMsg(EXIT_FAILURE, errorMsg);
	}

	totalBytesToRead = length;

	//read and count characters
	while (totalBytesToRead > 0) {
		if (totalBytesToRead > BUFFER_SIZE) {
			bytesToRead = BUFFER_SIZE;
		} else {
			bytesToRead = totalBytesToRead;
		}

		bytesRead = read(fd, buffer, bytesToRead);
		if (bytesRead < 0) {
			sscanf(errorMsg, "Error: Canno't read from file: %s",
					strerror(errno));
			exitMsg(EXIT_FAILURE, errorMsg);
		}

		if (bytesRead != bytesToRead) {
			sscanf(errorMsg,
					"Error: Not enough bytes read. read %d bytes out of %d",
					&bytesRead, &bytesToRead);
			exitMsg(EXIT_FAILURE, errorMsg);
		}

		count += countChar(charToCount, buffer, strlen(buffer));

		totalBytesToRead -=bytesRead;

	}

	return 0;

}

int countChar(char charToCount, char buffer[], int bufferSize) {
	int count = 0;

	for (int i = 0; i < bufferSize; i++) {
		if (buffer[i] == charToCount)
			count++;
	}

	return count;
}

void exitMsg(int exitCode, char* message) {
	printf("%s\n", message);
	exit(exitCode);
}
