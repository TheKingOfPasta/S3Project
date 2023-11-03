#pragma once
#include <math.h>
#include <stdlib.h>

typedef struct NodeLine {
	struct NodeLine * next;
	double rho;
	int theta;
} NodeLine;

typedef struct ListLine {
	NodeLine* head;
	int size;
} ListLine;

typedef struct Point {
	int x;
	int y;
} Point;

typedef struct NodeQuadrilateral{
	struct NodeQuadrilateral * next;
	Point p1,p2,p3,p4;
} NodeQuadrilateral;


typedef struct ListQuad {
	NodeQuadrilateral* head;
} ListQuad;

void Preppend(ListLine* l, NodeLine* nd);
void FreeList(ListLine *l);
double DoubleAbs(double x);
int CloseAngle(int t1, int t2, int threshold);
double ToRad(int t);