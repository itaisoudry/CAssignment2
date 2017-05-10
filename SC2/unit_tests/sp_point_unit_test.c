#include "../SPPoint.h"
#include "unit_test_util.h"
#include <stdbool.h>

//Checks if copy Works
bool pointBasicCopyTest() {
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	SPPoint* p = spPointCreate(data, dim, index);
	SPPoint* q = spPointCopy(p);
	//make sure the copy is not pointing to the source.
	ASSERT_TRUE(p != q);
	ASSERT_TRUE(spPointGetIndex(p) == spPointGetIndex(q));
	ASSERT_TRUE(spPointGetDimension(p) == spPointGetDimension(q));
	for (int i = 0; i < spPointGetDimension(p); i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p, i) == spPointGetAxisCoor(q, i));
	}
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}
bool pointBasicL2Distance() {
	double data1[2] = { 1.0, 1.0 };
	double data2[2] = { 1.0, 0.0 };
	int dim1 = 2;
	int dim2 = 2;
	int index1 = 1;
	int index2 = 1;
	SPPoint* p = spPointCreate(data1, dim1, index1);
	SPPoint* q = spPointCreate(data2, dim2, index2);
	ASSERT_TRUE(spPointL2SquaredDistance(p, p) == 0.0);
	ASSERT_TRUE(spPointL2SquaredDistance(q, q) == 0.0);
	ASSERT_FALSE(spPointL2SquaredDistance(p, q) == 0.0);
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}

bool dimensionAndIndexTest() {
	double data[3] = { 1.0, 5.0, -7.9 };
	int dim = 3;
	int index = 1;
	SPPoint* p = spPointCreate(data, dim, index);
	ASSERT_TRUE(spPointGetDimension(p) == dim);
	ASSERT_TRUE(spPointGetIndex(p) == 1);
	//free memory
	spPointDestroy(p);
	return true;
}
bool coordinateTest() {
	double data[3] = { 1.0, 2.5, 0.9 };
	int dim = 3;
	int index = 1, i;
	SPPoint* p = spPointCreate(data, dim, index);
	for (i = 0; i < dim; i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p, i) == data[i]);
	}
	spPointDestroy(p);
	return true;
}
bool invalidInputTest() {
	double data[3] = { 0.0, -4.5, 3.0 };
	int dim = 3;
	int index = 1;
	ASSERT_TRUE(spPointCreate(NULL, dim, index) == NULL);
	ASSERT_TRUE(spPointCreate(data,-1,index)==NULL);
	ASSERT_TRUE(spPointCreate(data,0,index)==NULL);
	ASSERT_TRUE(spPointCreate(data,dim,-7)==NULL);
	return true;
}
int main() {
	RUN_TEST(pointBasicCopyTest);
	RUN_TEST(pointBasicL2Distance);
	RUN_TEST(dimensionAndIndexTest);
	RUN_TEST(coordinateTest);
	RUN_TEST(invalidInputTest);
	return 0;
}
