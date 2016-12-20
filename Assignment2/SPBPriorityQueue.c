/*
 * SPBPriorityQueue.c
 *
 *  Created on: Dec 19, 2016
 *      Author: soudry
 */
#include "SPBPriorityQueue.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
typedef struct sp_bp_queue_t {
	int maxSize;
	int size;
	BPQueueElement *queue;
} SPBPQueue;

SPBPQueue* spBPQueueCreate(int maxSize) {
	SPBPQueue *newQueue = malloc(sizeof(SPBPQueue));
	newQueue->maxSize = maxSize;
	newQueue->queue = malloc(maxSize * sizeof(BPQueueElement));
	return newQueue;
}

SPBPQueue* spBPQueueCopy(SPBPQueue* source) {
	SPBPQueue *copiedQueue = malloc(sizeof(SPBPQueue));
	memcpy(&copiedQueue, &source, sizeof(SPBPQueue));
	return copiedQueue;
}

void spBPQueueDestroy(SPBPQueue* source) {
	free(source->queue);
	free(source);
}

void spBPQueueClear(SPBPQueue* source) {
	free(source->queue);
	source->queue = malloc(sizeof(BPQueueElement));
}

int spBPQueueSize(SPBPQueue* source) {
	return source->size;
}

int spBPQueueGetMaxSize(SPBPQueue* source) {
	return source->maxSize;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value) {
	SP_BPQUEUE_MSG msg = -1;
	int i = 0, swapIndex, size;
	bool found = false;
	BPQueueElement *newElement;
	source->size = source->size + 1;
	if ((source == NULL) || (index < 1) || (value < 0)) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	newElement->index = index;
	newElement->value = value;
	size = source->size;
	swapIndex = size;
	if (size == source->maxSize) {
		if (value > (source->queue[size - 1]).value) {
			return SP_BPQUEUE_FULL;
		}
		if (value == (source->queue[size - 1]).value) {
			(source->queue[size - 1]) = *newElement;
			return SP_BPQUEUE_SUCCESS;
		}
		swapIndex = size - 1;
	}
	//while bigger value not found and not finished iterating over array
	while (i < size && !found) {
		//if bigger value found
		if (source->queue[i].value > value) {
			//shift array to the right by one
			while (swapIndex >= i) {
				source->queue[swapIndex] = source->queue[swapIndex + 1];
				swapIndex--;
			}
			source->queue[i] = *newElement;
			//break loop
			found = true;
		}
		i++;
	}
	if (!found) {
		source->queue[i + 1] = *newElement;
	}

	return SP_BPQUEUE_SUCCESS;

}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source) {
	source->size = source->size - 1;
}

SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res) {

}

SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res) {

}

double spBPQueueMinValue(SPBPQueue* source) {

}

double spBPQueueMaxValue(SPBPQueue* source) {

}

bool spBPQueueIsEmpty(SPBPQueue* source) {

}

bool spBPQueueIsFull(SPBPQueue* source) {

}

