/*
 * example_unit_test.c
 *
 *  Created on: Dec 31, 2016
 *      Author: soudry
 */
#include "../SPBPriorityQueue.h"
#include "../SPPoint.h"
#include "unit_test_util.h"
#include <stdbool.h>
#include <stdlib.h>
//ignore this "unit test". it was just to make sure the example in the assignment works
bool exampleTest() {
	double value;
	int queueSize, pointsIndex;
	double data1[3] = { 1.0, 3.4, 63.1 };
	double data2[3] = { 1.2, 3.4, 0.1 };
	double data3[3] = { 1.0, 3.4, 0.1 };
	double data4[3] = { -123, 1234, 123 };
	double data5[3] = { 1.0, 3.4, 0.1 };
	double data6[3] = { 1.0, 3.4, 0.1 };
	SPBPQueue* queue = spBPQueueCreate(2);
	SPPoint** pointsArray = (SPPoint**) malloc(5 * sizeof(SPPoint*));
	SPPoint* p1 = spPointCreate(data1, 3, 1);
	SPPoint* p2 = spPointCreate(data2, 3, 2);
	SPPoint* p3 = spPointCreate(data3, 3, 3);
	SPPoint* p4 = spPointCreate(data4, 3, 4);
	SPPoint* p5 = spPointCreate(data5, 3, 5);
	SPPoint* q = spPointCreate(data6, 3, 1);
	pointsArray[0] = p1;
	pointsArray[1] = p2;
	pointsArray[2] = p3;
	pointsArray[3] = p4;
	pointsArray[4] = p5;
	for (pointsIndex = 0; pointsIndex < 5; pointsIndex++) {
		value = spPointL2SquaredDistance(pointsArray[pointsIndex], q);
		spBPQueueEnqueue(queue, pointsIndex, value);
	}
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

	return true;
}

int main4() {
	RUN_TEST(exampleTest);
	return 0;
}

