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

bool endToEndTest() {
	int index = 0;
	double data[] = { 1.0, 2.1, -4.7, 2.1, 7.0 };
	SP_BPQUEUE_MSG resultMsg;
	SPBPQueue* queue = spBPQueueCreate(5);
	for (index = 0; index < 5; index++) {
		resultMsg = spBPQueueEnqueue(queue, index, data[index]);
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
	for (index = 0; index < 5; index++) {
		resultMsg = spBPQueuePeek(queue, first);
		printf("%lf\n", first->value);
		resultMsg = spBPQueueDequeue(queue);
		ASSERT_TRUE(resultMsg == SP_BPQUEUE_SUCCESS);
	}

	free(first);
	free(last);
	spBPQueueDestroy(queue);
	return true;

}

int main() {
	if (endToEndTest()) {
		printf("PASS");
	}
	return 0;
}

