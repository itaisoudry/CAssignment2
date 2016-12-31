/*
 * main_aux.c
 *
 *  Created on: Dec 30, 2016
 *      Author: soudry
 */
#include "main_aux.h"

SPPoint** getUserInput(int n, int d) {
	int parametersIndex, pointsIndex;
	SPPoint** pointsArray = (SPPoint**) malloc(n * sizeof(SPPoint*));
	for (pointsIndex = 0; pointsIndex < n; pointsIndex++) {
		double data[d];
		printf("Please enter %d coordinates for this point", d);
		for (parametersIndex = 0; parametersIndex < d; parametersIndex++) {
			scanf("%lf", &data[parametersIndex]);
		}
		pointsArray[pointsIndex] = spPointCreate(data, d, pointsIndex);
	}

	return pointsArray;
}

SPPoint* getQPoint(int d) {
	int parametersIndex = 0;
	double data[d];
	SPPoint* q;
	printf("Please enter %d coordinates for the q point", d);
	for (parametersIndex = 0; parametersIndex < d; parametersIndex++) {
		scanf("%lf", &data[parametersIndex]);
	}
	//q don't need any index
	q = spPointCreate(data, d, 0);
	return q;

}

void enqueueElements(SPBPQueue* queue, SPPoint** pointsArray, SPPoint* q, int n) {
	int pointsIndex;
	double value;
	//iterating over points and calculate distance
	for (pointsIndex = 0; pointsIndex < n; pointsIndex++) {
		value = spPointL2SquaredDistance(pointsArray[pointsIndex], q);
		spBPQueueEnqueue(queue, pointsIndex, value);
	}
}

void printQueue(SPBPQueue* queue) {
	int queueSize, pointsIndex;
	queueSize = spBPQueueSize(queue);
	BPQueueElement *result = malloc(sizeof(BPQueueElement));
	for (pointsIndex = 0; pointsIndex < queueSize; pointsIndex++) {
		//peek
		spBPQueuePeek(queue, result);
		spBPQueueDequeue(queue);
		//print
		printf("%d", result->index + 1);
		if (pointsIndex < queueSize - 1) {
			printf(",");
		}
	}
	printf("\n");
	free(result);
}
