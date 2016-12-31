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

struct sp_point_t {
	int index;
	int dim;
	double* data;

};

SPPoint* spPointCreate(double* data, int dim, int index) {
	if (index < 0 || dim <= 0|| data==NULL) {
		return NULL;
	}
	SPPoint *point = (SPPoint*) malloc(sizeof(SPPoint));
	if (point == NULL) {
		return NULL;
	}
	point->index = index;
	point->dim = dim;
	point->data = (double*) malloc(dim * sizeof(double));
	if (point->data == NULL) {
		return NULL;
	}
	memcpy(point->data, data, dim * sizeof(double));
	return point;

}
SPPoint* spPointCopy(SPPoint* source) {
	assert(source!=NULL);
	//if allocation failed, point will be NULL, and this function will return NULL as requested.
	SPPoint *point = spPointCreate(source->data, source->dim, source->index);
	return point;
}
void spPointDestroy(SPPoint* point) {
	if (point != NULL) {
		free(point->data);
		free(point);
	}
}
int spPointGetDimension(SPPoint* point) {
	assert(point!=NULL);
	return point->dim;
}
int spPointGetIndex(SPPoint* point) {
	assert(point!=NULL);
	return point->index;
}
double spPointGetAxisCoor(SPPoint* point, int axis) {
	assert(point!=NULL);
	assert(axis < point->dim);
	return point->data[axis];
}
double spPointL2SquaredDistance(SPPoint* p, SPPoint* q) {
	assert(p!=NULL);
	assert(q!=NULL);
	assert(p->dim == q->dim);
	int index = 0;
	int size = p->dim;
	double sum = 0;
	for (; index < size; index++) {
		sum += ((q->data[index] - p->data[index])
				* (q->data[index] - p->data[index]));
	}
	return sum;
}
