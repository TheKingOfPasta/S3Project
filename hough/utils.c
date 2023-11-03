#include "utils.h"

// the data is allocated
void Preppend(List* l, void* data){
	Node * nd = malloc(sizeof(Node));
    nd->next = l->head;
    nd->data = data;
	l->head = nd;
	l->length++;
}

void FreeList(List *l)
{
    Node* curr = l->head;

    while (curr)
    {
        Node* prev = curr;
        curr = curr->next;
        free(prev->data);
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