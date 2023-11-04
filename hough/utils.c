#include "utils.h"

// the data is allocated
void Preppend(List* l, void* data){
	Node * nd = malloc(sizeof(Node));
    nd->next = l->head;
    nd->data = data;
	l->head = nd;
	l->length++;
}

void FreeNode(Node * nd){
    free(nd->data);
    free(nd);
}

void FreeList(List *l)
{
    Node* curr = l->head;

    while (curr)
    {
        Node* prev = curr;
        curr = curr->next;
        FreeNode(prev);
    }

    free(l);
}

void RemoveNextNode(List *l , Node *nd){
    Node* freeing = nd->next;
	nd->next = nd->next->next;
    l->length--;
    FreeNode(freeing);

}

void Tail(List *l )
{
    if(!l->head) return;
    Node* temp = l->head;
    l->head = temp->next;
    l->length--;
    FreeNode(temp);
}

int CloseAngle(double t1, double t2, double threshold){
    double diff = fmod( fabs(t1-t2),M_PI);
	return  diff< threshold || M_PI-diff< threshold;
}

double ToRad(int t) {
	return  t*M_PI/180.0;
}

int PointEquals(Point* p1, Point* p2){
	return p1->x ==p2->x && p1->y == p2->y;
}

int QuadEquals(Quadrilateral* q1,Quadrilateral* q2){
	return 	PointEquals(&(q1->p1),&(q2->p1)) &&
		 	PointEquals(&(q1->p2),&(q2->p2)) &&
		 	PointEquals(&(q1->p3),&(q2->p3)) &&
		 	PointEquals(&(q1->p4),&(q2->p4)) ;
}

int ContainsQuad(List* lquad, Quadrilateral* quad){
	Node* curr = lquad->head;
	while(curr){
		if (QuadEquals(quad,((Quadrilateral*)(curr->data))))
			return 1;
        curr =curr->next;
	}
	return 0;
}

Quadrilateral* InitializeQuad(Point* p1, Point* p2, Point* p3, Point* p4){
	Quadrilateral *quad =malloc(sizeof(Quadrilateral));

	double l1 =sqrt(p1->x*p1->x+p1->y*p1->y);
	double l2 =sqrt(p2->x*p2->x+p2->y*p2->y);
	double l3 =sqrt(p3->x*p3->x+p3->y*p3->y);
	double l4 =sqrt(p4->x*p4->x+p4->y*p4->y);
	if(l1<=l2 && l1<=l3 && l1<=l4){
		quad->p1 = *p1;
		quad->p2 = *p2;
		quad->p3 = *p3;
		quad->p4 = *p4;
	}
	else if(l2<=l3 && l2<=l4){
		quad->p1 = *p2;
		quad->p2 = *p3;
		quad->p3 = *p4;
		quad->p4 = *p1;
	}
	else if(l3<=l4){
		quad->p1 = *p3;
		quad->p2 = *p4;
		quad->p3 = *p1;
		quad->p4 = *p2;
	}
	else{
		quad->p1 = *p4;
		quad->p2 = *p1;
		quad->p3 = *p2;
		quad->p4 = *p3;
	}
	return quad;
}