#include "square_detection.h"

int FindIntersection(NodeLine* l1 ,NodeLine* l2, int w, int h, int *x, int *y ){
	//lines are not parallel
	float cost1=cosf(ToRad(l1->theta));
    float cost2=cosf(ToRad(l2->theta));
    float sint1=sinf(ToRad(l1->theta));
    float sint2=sinf(ToRad(l2->theta));
    float a=cost1*sint2-sint1*cost2;
    *x = (int)((sint2*l1->rho - sint1*l2->rho)/a);
	*y = (int)((cost1*l2->rho - cost2*l1->rho)/a);
	//check if the intersection is not out of bound
	return *x >0 && *y >0 && *x<w && *y<h;
}

//returns a list of the quadrilateral formed with the provided list of lines
ListQuad* FindSquares(ListLine* l,int width, int height){

	Point intersection[l->size][l->size];

	NodeLine *curr = l->head;
	int index = 0;

	Point p;

	while(curr){
		int angle = curr->theta +90 %180;
		NodeLine *innerCurr = l->head;
		int innerIndex =0;
		while(innerCurr){
			if( CloseAngle(angle,innerCurr->theta,30) &&
			 FindIntersection(curr,innerCurr,width,height,&(p.x),&(p.y))){
				intersection[index][innerIndex] = p;
			}
			else {
				intersection[index][innerIndex] = (Point){x:-1,y:-1};
			}
			innerIndex++;
			innerCurr= innerCurr->next;
		}
		curr = curr->next;
		index++;
	}

	/*
	//debug DONT RMV PLS
	printf("----");
	for (int j = 0; j < l->size; j++)
		printf("[%2i]        ",j);
	printf("\n");
	for (int i =0 ; i < l->size; i++)
	{
		printf("[%2i]",i);
		for (int j = 0; j < l->size; j++)
		{
			printf("x:%3i y:%3i ",intersection[i][j].x,intersection[i][j].y);
		}
		printf("\n");
	}
	*/

	ListQuad *lquad = malloc(sizeof(ListQuad));
	lquad->head = NULL;
	for(int i1 = 0; i1<l->size;i1++)
		for(int i2 = 0; i2<l->size;i2++)
			if(intersection[i1][i2].x !=-1)
				for (int i3 = 0; i3 < l->size; i3++)
					if(i3!=i1 && intersection[i2][i3].x !=-1)
						for (int i4 = 0; i4 < l->size; i4++)
							if(i4!=i2 && intersection[i3][i4].x !=-1 &&
								intersection[i4][i1].x != -1){
								NodeQuadrilateral *nd =
									malloc(sizeof(NodeQuadrilateral));
								nd->next = lquad->head;
								nd->p1 = intersection[i1][i2];
								nd->p2 = intersection[i2][i3];
								nd->p3 = intersection[i3][i4];
								nd->p4 = intersection[i4][i1];
								lquad->head = nd;
							}

	return lquad;
}


double Min4(double a,double b,double c,double d){
	return (a < b ? (a < c ? (a < d ? a : d) : (c < d ? c : d)) :
					(b < c ? (b < d ? b : d) : (c < d ? c : d)));
}

double Max4(double a,double b,double c,double d){
	return (a > b ? (a > c ? (a > d ? a : d) : (c > d ? c : d)) :
					(b > c ? (b > d ? b : d) : (c > d ? c : d)));
}

double Dist(Point p1, Point p2)
{
    double dX = p1.x - p2.x;
    double dY = p1.y - p2.y;
    return sqrt(dX*dX + dY*dY);
}

// returns the best square among a quadrilateral list
NodeQuadrilateral* BestSquare(ListQuad *l){
	if(!l->head) err(EXIT_FAILURE,"empty quad list\n");

	NodeQuadrilateral* currQuad = l->head;
	NodeQuadrilateral* bestSquare = l->head;
	double bestRatio = 0;

	while(currQuad){
		Point p1 = currQuad->p1;
    	Point p2 = currQuad->p2;
    	Point p3 = currQuad->p3;
    	Point p4 = currQuad->p4;
		double l1 = Dist(p1,p2);
		double l2 = Dist(p2,p3);
		double l3 = Dist(p3,p4);
		double l4 = Dist(p4,p1);
		double min = Min4(l1,l2,l3,l4);
		double max = Max4(l1,l2,l3,l4);
		min = min*min;
		max = max*max;
		double areaRatio  = (max - min)/max *min;
		double selectionRatio =min * 3 - areaRatio * 10;
		if(selectionRatio> bestRatio){
			bestRatio = selectionRatio;
			bestSquare = currQuad;
		}
		currQuad = currQuad->next;
	}
	return bestSquare;
}
