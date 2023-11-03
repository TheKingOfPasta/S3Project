#include "utils.h"

void Preppend(ListLine* l, NodeLine* nd){
	nd->next = l->head;
	l->head = nd;
	l->size++;
}

void FreeList(ListLine *l)
{
    NodeLine* curr = l->head;

    while (curr)
    {
        NodeLine* prev = curr;
        curr = curr->next;
        free(prev);
    }
    
    free(l);
}

double DoubleAbs(double x)
{
	return x < 0 ? -x : x;
}

int CloseAngle(int t1, int t2, int threshold){
	return (threshold + abs(t1 - t2) % 180) < (threshold<<1);
}

double ToRad(int t) {
	return  t*M_PI/180.0;
}