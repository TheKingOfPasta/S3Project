#pragma once
#include <math.h>
#include <stdlib.h>

typedef struct Line{
	double rho;
	int theta;
} Line;

typedef struct Point {
	int x;
	int y;
} Point;

typedef struct Quadrilateral{
	Point p1,p2,p3,p4;
} Quadrilateral;

typedef struct Node{
	struct Node* next;
	void * data;
} Node;

typedef struct List{
	Node* head;
	int length;
} List ;

/*
typedef struct NodeLine {
	struct NodeLine * next;
	double rho;
	int theta;
} NodeLine;

typedef struct ListLine {
	NodeLine* head;
	int size;
} ListLine;


typedef struct NodeQuadrilateral{
	struct NodeQuadrilateral * next;
	Point p1,p2,p3,p4;
} NodeQuadrilateral;


typedef struct ListQuad {
	NodeQuadrilateral* head;
} ListQuad;
*/

void Preppend(List* l, void* value);
void FreeList(List *l);
double DoubleAbs(double x);
int CloseAngle(int t1, int t2, int threshold);
double ToRad(int t);
int ContainsQuad(List* lquad, Quadrilateral* quad);
Quadrilateral* InitializeQuad(Point* p1, Point* p2, Point* p3, Point* p4);