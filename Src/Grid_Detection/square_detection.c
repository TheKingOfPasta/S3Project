#include "Grid_Detection/square_detection.h"

double Min4(double a,double b,double c,double d)
{
    return (a < b ? (a < c ? (a < d ? a : d) : (c < d ? c : d)) :
            (b < c ? (b < d ? b : d) : (c < d ? c : d)));
}

double Max4(double a,double b,double c,double d)
{
    return (a > b ? (a > c ? (a > d ? a : d) : (c > d ? c : d)) :
            (b > c ? (b > d ? b : d) : (c > d ? c : d)));
}

double Dist(Point p1, Point p2)
{
    double dX = p1.x - p2.x;
    double dY = p1.y - p2.y;
    return sqrt(dX*dX + dY*dY);
}


int FindIntersection(Line* l1,Line* l2, int w, int h, int *x, int *y,
                     double padding)
{
    //lines are not parallel
    float cost1=cosf((l1->theta));
    float cost2=cosf((l2->theta));
    float sint1=sinf((l1->theta));
    float sint2=sinf((l2->theta));
    float a=cost1*sint2-sint1*cost2;
    *x = (int)((sint2*l1->rho - sint1*l2->rho)/a);
    *y = (int)((cost1*l2->rho - cost2*l1->rho)/a);
    //check if the intersection is not out of bound
    return *x >-w*padding && *y >-h*padding && *x<w*(1+padding) && *y<h*(1+padding);
}



//returns a list of the quadrilateral formed with the provided list of lines
List* FindSquares(List* lHor, List* lVer,int width, int height,
                  double paddingPercentage)
{
    Point intersection[lVer->length][lHor->length];
    Node *currVer = lVer->head;
    Node *currHor;
    Point p;
    int index = 0;
    while(currVer) {
        currHor = lHor->head;
        Line* lineVer = ((Line*)(currVer->data));
        int innerIndex =0;
        while(currHor) {
            if( FindIntersection(lineVer,(Line*)(currHor->data)
                                 ,width,height,&(p.x),&(p.y), paddingPercentage/200)) {
                intersection[index][innerIndex] = p;
            } else {
                intersection[index][innerIndex] = (Point) {
x:
-1,y:
                        -1
                    };
            }
            innerIndex++;
            currHor= currHor->next;
        }
        currVer = currVer->next;
        index++;
    }
    /* 	//debug DONT RMV PLS
    	printf("----");
    	for (int j = 0; j < lVer->length; j++)
    		printf("[%2i]        ",j+1);
    	printf("\n");
    	for (int i =0 ; i < lHor->length; i++)
    	{
    		printf("[%2i]",i+1);
    		for (int j = 0; j < lVer->length; j++)
    		{
    			printf("x:%3i y:%3i ",intersection[j][i].x,intersection[j][i].y);
    		}
    		printf("\n");
    	}
    	printf( " Lver%i  Lhor%i \n",lVer->length,lHor->length);
    */
    Point p1,p2,p3,p4;
    List *lquad = InitList();
    for(int i1 = 0; i1<lVer->length; i1++) {
        //printf("i1: %i\n",i1);
        for(int i2 = 0; i2<lHor->length; i2++)
            if((p1 = intersection[i1][i2]).x !=-1) {
                //printf("	i2: %i\n",i2);
                for (int i3 = i1+1; i3 < lVer->length; i3++)
                    if((p2=intersection[i3][i2]).x !=-1) {
                        //printf("		i3: %i\n",i3);
                        for (int i4 = i2+1; i4 < lHor->length; i4++)
                            if((p3=intersection[i3][i4]).x !=-1 &&
                                    (p4=intersection[i1][i4]).x != -1) {
                                Quadrilateral *quad =InitializeQuad(&p1,&p2,&p3,&p4);
                                if(!ContainsQuad(lquad,quad)) {
                                    //printf("			i4: %i\n",i4);
                                    Preppend(lquad,quad);
                                    //printQuad(quad);
                                }
                            }
                    }
            }
    }
    return lquad;
}



// returns the best square among a quadrilateral list
Quadrilateral* BestSquare(List *l)
{
    if(!l->head) err(EXIT_FAILURE,"empty quad list\n");
    Quadrilateral* bestSquare = l->head->data;
    Node* curr = l->head;
    double bestRatio = 0;
    while(curr) {
        Quadrilateral* currQuad = curr->data;
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
        double selectionRatio =min * 5 - areaRatio * 10;
        if(selectionRatio> bestRatio) {
            bestRatio = selectionRatio;
            bestSquare = currQuad;
        }
        curr = curr->next;
    }
    return bestSquare;
}
