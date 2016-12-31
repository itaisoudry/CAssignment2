/*
 * main.c
 *
 *  Created on: Dec 27, 2016
 *      Author: soudry
 */
#include "main_aux.h"
int main13() {
	int n, d, k;
	//will be initialized when the user inputs data
	SPPoint** pointsArray;
	SPPoint* q;
	SPBPQueue* queue;

	printf("Input: ");
	scanf("%d %d %d", &n, &d, &k);
	//get points from user
	pointsArray  = getUserInput(n, d);
	q=getQPoint(d);
	//create queue
	queue = spBPQueueCreate(k);

	//enqueue elements with L2SquaredDistance
	enqueueElements(queue,pointsArray,q,n);

	//print queue
	printQueue(queue);

	return 0;
}
