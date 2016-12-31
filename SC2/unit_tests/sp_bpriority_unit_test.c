/*
 * sp_bpriority_unit_test.c
 *
 *  Created on: Dec 30, 2016
 *      Author: soudry
 */
#include "../SPBPriorityQueue.h"
#include "unit_test_util.h"
#include <stdbool.h>
#include <stdlib.h>
/**
 * End to end test.
 * Testing SPBQueue methods:
 * Create
 * Enqueue
 * isFull
 * size
 * getMaxSize
 * peek
 * peekLast
 * minValue
 * maxValue
 * isEmpty
 * Dequeue
 * Destroy
 *
 */
bool endToEndTest() {
	int index = 0;
	double data[] = { 1.0, 2.1, -4.7, 2.1, 7.0 };
	double sortedData[] = { -4.7, 1.0, 2.1, 2.1, 7.0 };
	SP_BPQUEUE_MSG resultMsg;
	SPBPQueue* queue = spBPQueueCreate(5);
	for (index = 0; index < 5; index++) {
		resultMsg = spBPQueueEnqueue(queue, index + 1, data[index]);
		ASSERT_TRUE(resultMsg == SP_BPQUEUE_SUCCESS);
	}
	ASSERT_TRUE(spBPQueueIsFull(queue)==true);
	ASSERT_TRUE(spBPQueueSize(queue) == 5);
	ASSERT_TRUE(spBPQueueGetMaxSize(queue) == spBPQueueSize(queue));
	BPQueueElement* last = (BPQueueElement*) malloc(sizeof(BPQueueElement));
	BPQueueElement* first = (BPQueueElement*) malloc(sizeof(BPQueueElement));
	resultMsg = spBPQueuePeek(queue, first);
	ASSERT_TRUE(resultMsg == SP_BPQUEUE_SUCCESS);
	resultMsg = spBPQueuePeekLast(queue, last);
	ASSERT_TRUE(resultMsg == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(first->value == -4.7);
	ASSERT_TRUE(last->value == 7.0);
	ASSERT_TRUE(spBPQueueMinValue(queue) == -4.7);
	ASSERT_TRUE(spBPQueueMaxValue(queue) == 7.0);

	//Dequeue and validate order of values
	for (index = 0; index < 5; index++) {
		resultMsg = spBPQueuePeek(queue, first);
		ASSERT_TRUE(first->value == sortedData[index]);
		resultMsg = spBPQueueDequeue(queue);
		ASSERT_TRUE(resultMsg == SP_BPQUEUE_SUCCESS);
	}
	ASSERT_TRUE(spBPQueueIsEmpty(queue));
	ASSERT_TRUE(spBPQueueDequeue(queue) == SP_BPQUEUE_EMPTY);
	free(first);
	free(last);
	spBPQueueDestroy(queue);
	return true;

}
/**
 * Checking copy method and edge cases
 */
bool copyAndEdgeCasesTest() {
	int index;
	int size = 0;
	SP_BPQUEUE_MSG resultMsg;
	SPBPQueue* source = spBPQueueCreate(5);

	ASSERT_TRUE(spBPQueueIsEmpty(source));
	//not checking the enqueue method, it was checked in the end to end test.
	spBPQueueEnqueue(source, 1, 5);
	spBPQueueEnqueue(source, 6, 5.5);
	spBPQueueEnqueue(source, 6, 0);
	spBPQueueEnqueue(source, 20, -40);
	spBPQueueEnqueue(source, 3, 70.3);

	//should return full
	resultMsg = spBPQueueEnqueue(source, 2, 100);
	ASSERT_TRUE(resultMsg == SP_BPQUEUE_FULL);
	SPBPQueue* copyQueue = spBPQueueCopy(source);
	ASSERT_TRUE(copyQueue!=NULL && copyQueue!=source);
	size = spBPQueueSize(source);

	//check that the source and copy are equal
	BPQueueElement* sourceElement = (BPQueueElement*) malloc(
			sizeof(BPQueueElement));
	BPQueueElement* copyElement = (BPQueueElement*) malloc(
			sizeof(BPQueueElement));
	for (index = 0; index < size; index++) {
		ASSERT_TRUE(sourceElement->index == copyElement->index);
		ASSERT_TRUE(sourceElement->value == copyElement->value);
	}
	ASSERT_TRUE(spBPQueueIsFull(copyQueue) == SP_BPQUEUE_FULL);
	resultMsg = spBPQueueEnqueue(source, -1, 2.0);
	ASSERT_TRUE(resultMsg == SP_BPQUEUE_INVALID_ARGUMENT);

	//free memory
	free(sourceElement);
	free(copyElement);
	spBPQueueDestroy(copyQueue);
	spBPQueueDestroy(source);

	return true;
}


int main2() {
	RUN_TEST(endToEndTest);
	RUN_TEST(copyAndEdgeCasesTest);
	return 0;
}

