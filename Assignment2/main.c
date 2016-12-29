/*
 * main.c
 *
 *  Created on: Dec 27, 2016
 *      Author: soudry
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SPPoint.h"
#include "SPBPriorityQueue.h"

//int main() {
//	int n, d, k, pointsIndex, parametersIndex, queueSize;
//	double value;
//	SPPoint** pointsArray;
//	SPPoint* q;
//	SPBPQueue* queue;
//	printf("Input: ");
//	scanf("%d %d %d", &n, &d, &k);
//	pointsArray = (SPPoint**) malloc(n * sizeof(SPPoint*));
//	for (pointsIndex = 0; pointsIndex < n + 1; pointsIndex++) {
//		double data[d];
//		printf("Please enter %d coordinates for this point", d);
//		for (parametersIndex = 0; parametersIndex < d; parametersIndex++) {
//			scanf("%f", &data[parametersIndex]);
//		}
//		//reading into q after finishing creating all points.
//		if (pointsIndex == n) {
//			q = malloc(sizeof(SPPoint*));
//			q = spPointCreate(data, d, pointsIndex);
//		} else
//			pointsArray[pointsIndex] = spPointCreate(data, d, pointsIndex);
//	}
//	queue = spBPQueueCreate(k);
//	//iterating over points and calculate distance
//	for (pointsIndex = 0; pointsIndex < n; pointsIndex++) {
//		value = spPointL2SquaredDistance(pointsArray[pointsIndex], q);
//		spBPQueueEnqueue(queue, pointsIndex, value);
//	}
//	//print queue
//	queueSize = spBPQueueSize(queue);
//	for (pointsIndex = 0; pointsIndex < queueSize; pointsIndex++) {
//		//peek
//		BPQueueElement *result = malloc(sizeof(BPQueueElement));
//		spBPQueuePeek(queue, result);
//		spBPQueueDequeue(queue);
//		//print
//		printf("%d", &result->index);
//	}
//	printf("\n");
//	return 0;
//}
