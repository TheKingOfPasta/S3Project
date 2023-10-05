# include "helper.h"
# include <stdlib.h>
# include <stdio.h>
# include <err.h>
# include <string.h>

void print_help()
{
    printf("Usage: \n"
            "       --all :         all the tests\n"
            "       --shuffle:      all the shuffle related tests\n"

            );

}

void print_list(int *a, size_t n)
{
    printf("{ ");
    for (size_t i = 0; i < n; ++i)
    {
        printf("%i ", a[i]);
    }
    printf("}\n");
}


void test_shuffle()
{
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    print_list(a, 11);
    shuffle((void *)a, 11, sizeof(int));
    printf("After shuffle: ");
    print_list(a, 11);
}


void all()
{
    test_shuffle();
}



int main(int argc, char **argv)
{
    if (argc == 1)
    {
        print_help();
        return EXIT_SUCCESS;
    }
    if (argc != 2)
    {
        errx(EXIT_FAILURE, "Invalid usage. Expected 1 argument");
    }

    if (strcmp(argv[1], "--all") == 0)
    {
        printf("Starting test suite...\n");
        all();
    }

    return EXIT_SUCCESS;
}

