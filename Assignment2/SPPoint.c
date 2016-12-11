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
struct sp_point_t {
	int index;
	int dim;
	double* data;

};

SPPoint* spPointCreate(double* data, int dim, int index) {
	struct sp_point_t *point = malloc(sizeof(SPPoint));
	point->index = index;
	point->dim = dim;
	point->data = (double*) malloc(dim * sizeof(double));
	memcpy(point->data, data, dim * sizeof(double));
	return point;

}
SPPoint* spPointCopy(SPPoint* source) {
	struct sp_point_t *point = malloc(sizeof(SPPoint));
	memcpy(&point, &source, sizeof(SPPoint))
	return point;
}
void spPointDestroy(SPPoint* point) {
	free(point);
}
int spPointGetDimension(SPPoint* point) {
	return 0;
}
int spPointGetIndex(SPPoint* point) {
	return 0;
}
double spPointGetAxisCoor(SPPoint* point, int axis) {
	return 0;
}
double spPointL2SquaredDistance(SPPoint* p, SPPoint* q) {
	return 0;
}

void main(void) {
	double data[5] = { 1, 2, 3, 4, 5 };
	SPPoint *o;
	o = spPointCreate(data, 5, 1);
	spPointDestroy(o);
}
