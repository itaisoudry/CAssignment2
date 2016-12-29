/*
 * SPBPriorityQueue.c
 *
 *  Created on: Dec 19, 2016
 *      Author: soudry
 */
#include "SPBPriorityQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
typedef struct sp_bp_queue_t {
	int maxSize;
	int size;
	int start;
	BPQueueElement *queue;
} SPBPQueue;

SPBPQueue* spBPQueueCreate(int maxSize) {
	if (maxSize < 0) {
		return NULL;
	}
	SPBPQueue *newQueue = (SPBPQueue*) malloc(sizeof(SPBPQueue));
	if (newQueue == NULL) {
		return NULL;
	}
	newQueue->maxSize = maxSize;
	newQueue->start = 0;
	newQueue->queue = (BPQueueElement*) malloc(
			maxSize * sizeof(BPQueueElement));
	if (newQueue->queue == NULL) {
		return NULL;
	}
	return newQueue;
}

SPBPQueue* spBPQueueCopy(SPBPQueue* source) {
	SPBPQueue *copiedQueue = spQueueCreate(source->maxSize);
	return copiedQueue;
}

void spBPQueueDestroy(SPBPQueue* source) {
	if (source != NULL) {
		free(source->queue);
		free(source);
	}
}

void spBPQueueClear(SPBPQueue* source) {
	if (source != NULL) {
		free(source->queue);
		source->queue = (BPQueueElement*) malloc(sizeof(BPQueueElement));
		if (source->queue == NULL) {
			printf("Error allocating memory");
		}
	}
}

int spBPQueueSize(SPBPQueue* source) {
	if (source == NULL) {
		return -1;
	}
	return source->size;
}

int spBPQueueGetMaxSize(SPBPQueue* source) {
	if (source == NULL) {
		return -1;
	}
	return source->maxSize;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value) {
	int i = source->start, swapIndex, size;
	int maxSize = source->maxSize;
	bool found = false;
	BPQueueElement *newElement;
	if (source == NULL || index < 0) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	newElement = (BPQueueElement*) (sizeof(BPQueueElement));
	//we assume that if the allocation didn't succeed is cause of memory problems.
	if (newElement == NULL) {
		printf("Error allocating memory");
		return SP_BPQUEUE_OUT_OF_MEMORY;
	}
	if ((source == NULL) || (index < 1) || (value < 0)) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	source->size = source->size + 1;
	newElement->index = index;
	newElement->value = value;
	size = source->size;
	swapIndex = size;
	if (size == maxSize) {
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
	while ((i % maxSize) < size && !found) {
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
		i = (i + 1) % maxSize;
	}
	if (!found) {
		source->queue[i + 1] = *newElement;
	}

	return SP_BPQUEUE_SUCCESS;

}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source) {
	if (source == NULL)
		return SP_BPQUEUE_INVALID_ARGUMENT;
	if(source->size==0){
		return SP_BPQUEUE_EMPTY;
	}
	source->start = (source->start + 1) % source->maxSize;
	source->size = source->size - 1;
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res) {
	if (res == NULL || source == NULL) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if (source->size == 0) {
		return SP_BPQUEUE_EMPTY;
	}
	BPQueueElement* firstElement = &source->queue[source->start];
	*res = *firstElement;
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res) {
	if (res == NULL || source == NULL) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if (source->size == 0) {
		return SP_BPQUEUE_EMPTY;
	}
	BPQueueElement* lastElement = &source->queue[source->size];
	*res = *lastElement;
	return SP_BPQUEUE_SUCCESS;
}

double spBPQueueMinValue(SPBPQueue* source) {
	if (source == NULL) {
		printf("Invalid argument");
		return -1;
	}
	BPQueueElement *result = (BPQueueElement*) malloc(sizeof(BPQueueElement));
	if (result == NULL) {
		printf("Error allocating memory");
		return -1;
	}
	SP_BPQUEUE_MSG resultMsg = spBPQueuePeek(source, result);
	if (resultMsg != SP_BPQUEUE_SUCCESS)
		return -1;
	return result->value;
}

double spBPQueueMaxValue(SPBPQueue* source) {
	if (source == NULL) {
		printf("Invalid argument");
		return -1;
	}
	BPQueueElement *result = (BPQueueElement*) malloc(sizeof(BPQueueElement));
	if (result == NULL) {
		printf("Error allocating memory");
		return -1;
	}
	SP_BPQUEUE_MSG resultMsg = spBPQueuePeekLast(source, result);
	if (resultMsg != SP_BPQUEUE_SUCCESS)
		return -1;
	return result->value;
}

bool spBPQueueIsEmpty(SPBPQueue* source) {
	if (source == NULL) {
		printf("Invalid argument");
		return false;
	}
	return source->size == 0 ? true : false;
}

bool spBPQueueIsFull(SPBPQueue* source) {
	if (source == NULL) {
		printf("Invalid argument");
		return false;
	}
	return source->size == source->maxSize ? true : false;
}

