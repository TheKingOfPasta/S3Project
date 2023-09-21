//Method prefix : PSR

#include <stdio.h>

#define DEFAULT_CELL_VALUE 0b0011001111111111


void PSR_parse(char* s, short grid[9][9])
{
    size_t i = 0;
    size_t j = 0;
    size_t index = 0;

    while (s[index] != 0)//We stop at the null character that's at the end of string
    {
        if (s[index] == '\n')
        {
            //Ignore consecutive \n's
            while (s[index] == '\n')
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

        if (c != ' ' && c != '.' && (c < '1' || c > '9'))
        {
            printf("The given string was not a valid format (only dots, spaces, numbers and new lines are allowed), found %c (ascii %i)\n", c, c);
            return;
            //errx(EXIT_FAILURE, "The given string was not a valid format (only dots, spaces, numbers and new lines are allowed), found %c (ascii %i)\n", c, c);
        }

        if (c == '.')
        {
            grid[i][j] = DEFAULT_CELL_VALUE;
        }
        else if (c == ' ')
        {
            continue;
        }
        else//if (c >= '1' && c <= '9')
        {
            grid[i][j] = c - '0';
        }

        i += 1;
    }
}

void print(short m[9][9])
{
    for (size_t j = 0; j < 9; j++)
    {
        for (size_t i = 0; i < 9; i++)
        {
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

int main()
{
    short m[9][9];
    PSR_parse("... ..4 58.\n... 721 ..3\n4.3 ... ...\n\n21. .67 ..4\n.7. ... 2..\n63. .49 ..1\n3.6 ... ...\n\n... 158 ..6\n... ..6 95.\0", m);

    print(m);
}
