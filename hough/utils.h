#pragma once
#include <math.h>
#include <stdlib.h>

typedef struct Line{
	double rho;
	double theta;
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

void Preppend(List* l, void* value);
void FreeList(List *l);
void RemoveNextNode(List *l , Node *nd);
void Tail(List *l );
int CloseAngle(double t1, double t2, double threshold);
double ToRad(int t);
int ContainsQuad(List* lquad, Quadrilateral* quad);
Quadrilateral* InitializeQuad(Point* p1, Point* p2, Point* p3, Point* p4);