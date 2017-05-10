/*
 * main_aux.h
 *
 *  Created on: Dec 30, 2016
 *      Author: soudry
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SPPoint.h"
#include "SPBPriorityQueue.h"

/**
 * Gets input from the user according to the assignment. (number of d sized points parameters)
 * @return SPPoint** - array of pointers to SPPoint - array of points
 */
SPPoint** getUserInput(int n, int d);
/**
 * Gets the Q point from the user
 * @return SPPoint* - the Q point that the user chose.
 */
SPPoint* getQPoint(int d);
/**
 * Enqueue all elements in the queue.
 * Each elements has its own index and value ( value is the L2SquaredDisatance ), its been created and enqueued.
 */
void enqueueElements(SPBPQueue* queue, SPPoint** pointsArray, SPPoint* q,
		int n);
/**
 * Prints the k indexes that has the closest distance to the point q.
 */
void printQueue(SPBPQueue* queue);
#endif /* MAIN_AUX_H_ */
