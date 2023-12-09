//Method prefix : PSR
#include "Solver/parser.h"

#define DEFAULT_CELL_VALUE 0b0011001111111111

void PSR_parse(char* s, short grid[9][9])
{
    size_t i = 0;
    size_t j = 0;
    size_t index = 0;
    //We stop at the null character that's at the end of string
    while (s[index] != 0) {
        if (s[index] == '\n') {
            //Ignore consecutive \n's
            while (s[index] == '\n' || s[index] == '\r')
                index += 1;
            //Next line
            j += 1;
            i = 0;
            if (j == 9)
                return;
            continue;
        }
        char c = s[index];
        index += 1;
        if (c != ' ' && c != '.' && (c < '1' || c > '9')) {
            printf("The given string was not a valid format "
                   "(only dots, spaces, numbers and new lines are allowed),"
                   "found %c (ascii %i)\n", c, c);
            //Continue and no error because there are some things like
            // \r's at end of lines and things like that :/
            continue;
        }
        if (c == '.') {
            grid[i][j] = DEFAULT_CELL_VALUE;
        } else if (c == ' ') {
            continue;
        } else { //if (c >= '1' && c <= '9')
            grid[i][j] = c - '0';
        }
        i += 1;
    }
}

void PSR_parse_file(char* file, short grid[9][9])
{
    FILE* f = fopen(file, "r");
    if (!f) {
        printf("File not found!\n");
        return;
    }
    char* text = malloc(112);
    char c;
    size_t i = 0;
    do {
        c = fgetc(f);//Get next character in string
        text[i] = c;
        i += 1;
    } while (c != EOF);//EOF == end of file
    PSR_parse(text, grid);
    fclose(f);
}

char* PSR_unparse(short grid[9][9])
{
    char* s = malloc(111);
    size_t index = 0;
    for (size_t j = 0; j < 9; j++) {
        for (size_t i = 0; i < 9; i++) {
            //if grid[i][j] is larger than 9 => the cell hasn't been solved yet
            s[index] = (grid[i][j] > 9) ? '.' : (grid[i][j] + '0');
            index += 1;
            if (i % 3 == 2 && i != 8)//i == 8 <=> end of line
                s[index++] = ' ';
        }
        if (j % 3 == 2)
            s[index++] = '\n';
        s[index++] = '\n';
    }
    s[index - 1] = '\0';//-1 to replace the last \n (not needed apparently?)
    return s;
}

//Print sudoku grid
void print(short m[9][9])
{
    for (size_t j = 0; j < 9; j++) {
        for (size_t i = 0; i < 9; i++) {
            if (m[i][j] == DEFAULT_CELL_VALUE)
                printf(".");
            else
                printf("%hi", m[i][j]);
            if (i % 3 == 2)
                printf(" ");
        }
        printf("\n");
        if (j % 3 == 2)
            printf("\n");
    }
}