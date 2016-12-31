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

void printQueue(SPBPQueue* queue);
SPPoint** getUserInput(int n, int d);
SPPoint* getQPoint(int d);
void enqueueElements(SPBPQueue* queue, SPPoint** pointsArray, SPPoint* q,
		int n);

#endif /* MAIN_AUX_H_ */
