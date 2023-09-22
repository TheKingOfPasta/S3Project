#include <stdio.h>
#include "parser.h"

#define DEFAULT_CELL_VALUE 0b0011001111111111


void collapse_grid(short grid[9][9], int x, int y,short val);
short collapsed_cell_value(short cell_val);
void print(short grid[9][9]);
void printUnsolved(short x);


#define IsNotSolved(x) (x>>13)
#define IsSolved(x) (!(x>>13))
#define getNbPossibilities(x) ((x>>9) & 0b1111)
#define setNbPossibilities(x,nb) ((x & 0b0000111111111) + (nb <<9))
#define bumpMinusNbPossibilities(x) (x - (1<<9))
#define rmvPossibility(x,possib) (x & ~(1<<(possib-1)))

short rmv_possibility(short* cell_val,short possibility) {
    //returns true if the cell collapsed
    // otherwise false
    //removing the possibility of the cell value

    short res = rmvPossibility(*cell_val,possibility);
    if(res != (*cell_val))//check if it removed a possibility
    {
        if(  getNbPossibilities(*cell_val)==2){
            *cell_val = collapsed_cell_value(res);
            return 1;
        }
        else
            res = bumpMinusNbPossibilities(res);
    }
    *cell_val = res;
    return 0;
}


short collapsed_cell_value(short cell_val){
    //returns the value of the cell after collapsing
    short res = 1;
    while (! (cell_val & 0b1)) {
        cell_val >>=1;
        res++;
    }
    return res;
}



void collapse_cell(short grid[9][9],int x,int y,short val){
    if(IsNotSolved(grid[x][y])){
        //if (msg) printf("    x=%i,y=%i  rmvng %hi \n ",x,y,val);
        //if (msg) printf("    nb possibil=%hi\n",getNbPossibilities(grid[x][y]));
        //if (msg) printUnsolved(grid[x][y]);
        if(rmv_possibility(&grid[x][y],val)){
            //if (msg) printf("        collapsing on a %hi!! at x=%i,y=%i \n",grid[x][y],x,y);
            //if (msg) print(grid);
            collapse_grid(grid,x,y,grid[x][y]);
        }
        //else if(msg) printUnsolved(grid[x][y]);
    }
}

void collapse_grid(short grid[9][9], int x, int y,short val){
    //printf(" %hi for collapse pos x=%i,y=%i \n", val,x,y);

    //update cell group
    for(int i =x/3*3;i<(x+3)/3*3;i++){
        for(int j = y/3*3;j<(y+3)/3*3;j++)
        {
            collapse_cell(grid,i,j,val);
        }
    }
    for (int i = 0 ; i<9;i++){
        //update line x axis
        collapse_cell(grid,i,y,val);
        //update column y axis
        collapse_cell(grid,x,i,val);
    }
}

void wave_function_collapse(short grid[9][9]) {
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++){
            if(IsSolved(grid[i][j])){
                collapse_grid(grid,i,j,grid[i][j]);
            }
        }
}

void printUnsolved(short x){
    printf("             ");
    for(int i =1; i<10; i++) {
        printf("%i=%i,",i,x%2);
        x>>=1;
    }
    printf("\n");
}

void main()
{
    short m[9][9];

    PSR_parse_file("testfile.txt", m);

    print(m);

    wave_function_collapse(m);
    printf("\nwwwwwwwwwwwwwwwwwwwwwwwwwwww\n");
    print(m);
}

