#pragma once
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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
Node* PrepopNode(List* l);
void InsertNode(List * l , Node* nd);
List* InitList();
void FreeNode(Node* nd);
void FreeList(List* l);
void RemoveNextNode(List *l , Node *nd);
void Tail(List *l );
void mergeSort(struct Node** headRef);
int CloseAngle(double t1, double t2, double threshold);
double ToRad(int t);
int ContainsQuad(List* lquad, Quadrilateral* quad);
Quadrilateral* InitializeQuad(Point* p1, Point* p2, Point* p3, Point* p4);