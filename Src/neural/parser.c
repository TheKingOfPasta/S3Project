#include <stdio.h>
#include "Neural_Network/second_network.h"

int main(int argc, char** argv)
{
    if (argc != 2)
        return 1;

    FILE *file = fopen(argv[1], "r");
    char s[1000];
    fgets(s, sizeof(s), file);
    size_t n = 0;
    for (size_t i = 0; s[i] && s[i] != '\n'; i++)
    {
        n = n * 10 + s[i] - '0';
    }

    size_t arr[n];
    for (size_t i = 0; i < n; i++)
    {
        fgets(s, sizeof(s), file);
        // s is now the line
        size_t num = 0;
        for (size_t j = 0; s[j] && s[j] != '\n'; j++)
        {
            num = num * 10 + s[j] - '0';
        }
        arr[i] = num;
    }

    //Need to get the weights and biases now :(

    Network* nn = new_Network(arr, n);

    nn->biases[0];

    fclose(file);
}