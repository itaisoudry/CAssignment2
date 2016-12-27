/*
 * SPPoint.c
 *
 *  Created on: Dec 11, 2016
 *      Author: soudry
 */

#include "SPPoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

typedef struct sp_point_t {
	int index;
	int dim;
	double* data;

} SPPoint;

SPPoint* spPointCreate(double* data, int dim, int index) {
	if (index < 0 || dim <= 0) {
		return NULL;
	}
	SPPoint *point = malloc(sizeof(SPPoint));
	point->index = index;
	point->dim = dim;
	point->data = (double*) malloc(dim * sizeof(double));
	memcpy(point->data, data, dim * sizeof(double));
	return point;

}
SPPoint* spPointCopy(SPPoint* source) {
	SPPoint *point = malloc(sizeof(SPPoint));
	memcpy(&point, &source, sizeof(SPPoint));
	return point;
}
void spPointDestroy(SPPoint* point) {
	free(point->data);
	free(point);
}
int spPointGetDimension(SPPoint* point) {
	return point->dim;
}
int spPointGetIndex(SPPoint* point) {
	return point->index;
}
double spPointGetAxisCoor(SPPoint* point, int axis) {
	return point->data[axis];
}
double spPointL2SquaredDistance(SPPoint* p, SPPoint* q) {
	int index = 0;
	int size = sizeof(p->data) / sizeof(double);
	double sum = 0;
	for (; index < size; index++) {
		sum += ((p->data[index]) - (q->data[index]))
				* ((p->data[index]) - (q->data[index]));
	}
	return sum;
}

//void main(void) {
//	double data[5] = { 1, 2, 3, 4, 5 };
//	SPPoint *o, *o1;
//	o1 = spPointCreate(data, 5, 2);
//	o = spPointCreate(data, 5, 1);
//	printf("%f", spPointGetAxisCoor(o, 4));
//	printf("%f", spPointL2SquaredDistance(o, o1));
//	spPointDestroy(o);
//}
