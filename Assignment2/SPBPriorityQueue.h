#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include <stdbool.h>

/**
 * SP Bounded Priority Queue summary
 *
 * TODO Complete documentation
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t SPBPQueue;

typedef strcut sp_bpq_element_t {
	int index;
	double value;
} BPQueueElement;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * TODO Complete documentation
 */
SPBPQueue* spBPQueueCreate(int maxSize);

/**
 * TODO Complete documentation
 */
SPBPQueue* spBPQueueCopy(SPBPQueue* source);

/**
 * TODO Complete documentation
 */
void spBPQueueDestroy(SPBPQueue* source);

/**
 * TODO Complete documentation
 */
void spBPQueueClear(SPBPQueue* source);

/**
 * TODO Complete documentation
 */
int spBPQueueSize(SPBPQueue* source);

/**
 * TODO Complete documentation
 */
int spBPQueueGetMaxSize(SPBPQueue* source);

/**
 * TODO Complete documentation
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value);

/**
 * TODO Complete documentation
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source);

/**
 * TODO Complete documentation
 */
SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res);

/**
 * TODO Complete documentation
 */
SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res);

/**
 * TODO Complete documentation
 */
double spBPQueueMinValue(SPBPQueue* source);

/**
 * TODO Complete documentation
 */
double spBPQueueMaxValue(SPBPQueue* source);

/**
 * TODO Complete documentation
 */
bool spBPQueueIsEmpty(SPBPQueue* source);

/**
 * TODO Complete documentation
 */
bool spBPQueueIsFull(SPBPQueue* source);

#endif
