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
#include <stdbool.h>

#define MAX_LENGTH 1025

int main(int argc, char** argv) {
	char charToCount;
	char filePath[MAX_LENGTH];

	if (argc != 2) {
		exitMsg(EXIT_FAILURE, "Error: Invalid arguments");
	}

	if(strlen(argv[1]) != 1 || !isalpha(argv[1][0])){
		exitMsg(EXIT_FAILURE, "Error: Input should be exactly one character");
	}

	charToCount = argv[1][0];
	strcpy(filePath,argv[2]);



	return 0;
}

void exitMsg(int exitCode, char* message) {
	printf("%s\n", message);
	exit(exitCode);
}
