#include "Solver/solver.h"

#define DEFAULT_CELL_VALUE 0b0011001111111111

#define IsNotSolved(x) (x>>13)
#define IsSolved(x) (!(x>>13))
#define getNbPossibilities(x) ((x>>9) & 0b1111)
#define setNbPossibilities(x,nb) ((x & 0b0000111111111) + (nb <<9))
#define bumpMinusNbPossibilities(x) (x - (1<<9))
#define rmvPossibility(x,possib) (x & ~(1<<(possib-1)))
#define checkIfPossible(x,possib) (x>>(possib-1)%2)

int remainingCell;

short SLV_rmv_possibility(short* cell_val, short possibility) {
    //returns true if the cell collapsed
    // otherwise false
    //removing the possibility of the cell value

    short res = rmvPossibility(*cell_val,possibility);
    if(res != (*cell_val))//check if it removed a possibility
    {
        if(  getNbPossibilities(*cell_val)==2){
            *cell_val = SLV_get_collapsed_value(res);
            return 1;
        }
        else
            res = bumpMinusNbPossibilities(res);
    }
    *cell_val = res;
    return 0;
}

short SLV_get_collapsed_value(short cell_val){
    //returns the value of the cell after collapsing
    short res = 1;
    while (! (cell_val & 0b1)) {
        cell_val >>=1;
        res++;
    }
    return res;
}


void SLV_collapse_cell(short grid[9][9],int x,int y,short val){
    if(IsNotSolved(grid[x][y]))
    {
        if(SLV_rmv_possibility(&grid[x][y], val))
        {
            remainingCell--;
            if(remainingCell > 0) SLV_collapse_grid(grid, x, y, grid[x][y]);
        }
        //else if(msg) printUnsolved(grid[x][y]);
    }
}

void SLV_collapse_grid(short grid[9][9], int x, int y, short val){
    //printf(" %hi for collapse pos x=%i,y=%i \n", val,x,y);

    //update cell group
    for(int i =x/3*3;i<(x+3)/3*3;i++){
        for(int j = y/3*3;j<(y+3)/3*3;j++)
        {
            SLV_collapse_cell(grid,i,j,val);
        }
    }

    for (int i = 0 ; i<9;i++){
    //update line x axis
        SLV_collapse_cell(grid,i,y,val);
    //update column y axis
        SLV_collapse_cell(grid,x,i,val);
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

void SLV_Clues_Collapsing(short grid[9][9]) {
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++){
            if(remainingCell < 1) return;
            if(IsSolved(grid[i][j])){
                remainingCell--;
                SLV_collapse_grid(grid, i, j, grid[i][j]);
            }
        }
}


int SLV_is_Placeable(short g[9][9], int x, int y, short val) {
    //returns true if placement of guess is possible

    //check cell group
    for(int i =x/3*3;i<(x+3)/3*3;i++)
        for(int j = y/3*3;j<(y+3)/3*3;j++)
            if(g[i][j] == val) return 0;

    for (int i = 0 ; i<9;i++){
        //check line x
        if(g[i][y] == val) return 0;
        //check column y
        if(g[x][i] == val) return 0;
    }
    return 1;
}

void SLV_backtracking(short grid[9][9]){
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if(IsSolved(grid[i][j])) continue;

            short cp = grid[i][j];
            short initialVal = cp;
            for(short guessVal=1;guessVal<10;guessVal++){
                if(cp%2){
                    if(SLV_is_Placeable(grid, i, j, guessVal))
                    {
                        grid[i][j] = guessVal;
                        SLV_backtracking(grid);
                        if(remainingCell>0) grid[i][j] =initialVal;
                    }
                }
                else{
                    cp>>=1;
                }
            }
            return;
        }
    }
    remainingCell = 0;
}



void SLV_findRemainingCells(short g[9][9],short a[]){
    int index = 0;
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if(IsNotSolved(g[i][j]))
                a[index++] = i*9+j;
        }
    }
}

void SLV_solve(short grid[9][9]){
    remainingCell = 81;
    SLV_Clues_Collapsing(grid);
    if(remainingCell<1) return;
    printf("brute forcing now..\n");


    short remainingCellIndex[81] = {0}; //filled with zeros
    SLV_findRemainingCells(grid ,remainingCellIndex);
    //SLV_sort(remainingCellIndex);

    //looking_for_one_occurrence(grid);
    SLV_backtracking(grid);
}



/* int main(int argc, char *argv[])
{
	if (argc != 2)
    {
		errx(EXIT_FAILURE, "misuse of solver 1 param only : ./solver 'path'");
	}


    short m[9][9];

    char* path = argv[1];

    PSR_parse_file(path, m);

    SLV_solve(m);//m is now a correct matrix, save it into 'path'.result

    char* s = PSR_unparse(m);

    path = strcat(path, ".result");

    FILE *fptr = fopen(path, "w");

    //Write
    fprintf(fptr, s);

    fclose(fptr);

    free(s);//Useless but shrug

    return EXIT_SUCCESS;
}
 */