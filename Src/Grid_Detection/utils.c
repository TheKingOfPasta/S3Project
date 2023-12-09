#include "Grid_Detection/utils.h"

void PreppendNode(List* l, Node* nd)
{
    nd->next = l->head;
    l->head = nd;
    l->length++;
}

// the data is allocated
void Preppend(List* l, void* data)
{
    Node * nd = malloc(sizeof(Node));
    nd->data = data;
    PreppendNode(l,nd);
}

Node* PrepopNode(List* l)
{
    Node * nd  = l->head;
    l->head = nd->next;
    return nd;
}

void InsertNode(List* l, Node* nd)
{
    Node* curr  = l->head;
    double r = ((Line *)(nd->data))->rho;
    if(curr == NULL || ((Line *)(curr->data))->rho >r) {
        PreppendNode(l,nd);
        return;
    }
    l->length ++;
    while(curr->next && ((Line *)(curr->next->data))->rho<r)
        curr= curr->next;
    nd->next = curr->next;
    curr->next = nd;
}


List* InitList()
{
    List* list = malloc(sizeof(List));
    list->head = NULL;
    list->length =0;
    return list;
}

void FreeNode(Node * nd)
{
    free(nd->data);
    free(nd);
}

void FreeList(List *l)
{
    Node* curr = l->head;
    while (curr) {
        Node* prev = curr;
        curr = curr->next;
        FreeNode(prev);
    }
    free(l);
}

void RemoveNextNode(List *l, Node *nd)
{
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

int CloseAngle(double t1, double t2, double threshold)
{
    double diff = fmod( fabs(t1-t2),M_PI);
    return  diff< threshold || M_PI-diff< threshold;
}

double ToRad(int t)
{
    return  t*M_PI/180.0;
}

int PointEquals(Point* p1, Point* p2)
{
    return p1->x ==p2->x && p1->y == p2->y;
}

int QuadEquals(Quadrilateral* q1,Quadrilateral* q2)
{
    return 	PointEquals(&(q1->p1),&(q2->p1)) &&
            PointEquals(&(q1->p2),&(q2->p2)) &&
            PointEquals(&(q1->p3),&(q2->p3)) &&
            PointEquals(&(q1->p4),&(q2->p4)) ;
}

int ContainsQuad(List* lquad, Quadrilateral* quad)
{
    Node* curr = lquad->head;
    while(curr) {
        if (QuadEquals(quad,((Quadrilateral*)(curr->data))))
            return 1;
        curr =curr->next;
    }
    return 0;
}

Quadrilateral* InitializeQuad(Point* p1, Point* p2, Point* p3, Point* p4)
{
    Quadrilateral *quad =malloc(sizeof(Quadrilateral));
    quad->p1 = *p1;
    quad->p3 = *p3;
    if(abs(p1->x - p4->x) <abs(p1->x - p2->x)) {
        quad->p2 = *p2;
        quad->p4 = *p4;
    } else {
        quad->p4 = *p2;
        quad->p2 = *p4;
    }
    return quad;
}