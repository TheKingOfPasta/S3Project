# include "network_loader.h"

void Save_Network(Network* n, char* path)
{
    FILE* f = fopen(path, "w");
    //This empties the file
    fclose(f);
    f = fopen(path, "a");


    fprintf(f,"%zu\n", n->num_layers);
    for (DoubleNode* curr = n->sizes->head; curr; curr = curr->next)
    {
        fprintf(f,"%zu\n", curr->d);
    }

    for (Node_m* curr = n->biases->head; curr; curr = curr->next)
    {
        fprintf(f, "%zu\n", curr->m->w);
        fprintf(f, "%zu\n", curr->m->h);

        for (size_t j = 0; j < curr->m->h; j++)
        {
            for (size_t i = 0; i < curr->m->w; i++)
            {
                fprintf(f, "%f ", curr->m->values[i][j]);
            }
            fprintf(f, "\n");
        }
    }

    for (Node_m* curr = n->weights->head; curr; curr = curr->next)
    {
        fprintf(f, "%zu\n", curr->m->w);
        fprintf(f, "%zu\n", curr->m->h);

        for (size_t j = 0; j < curr->m->h; j++)
        {
            for (size_t i = 0; i < curr->m->w; i++)
            {
                fprintf(f, "%f ", curr->m->values[i][j]);
            }
            fprintf(f, "\n");
        }
    }

    fclose(f);
}

/// @brief The string is assumed correct and ends with a \n
size_t StrToSize_t(char* s)
{
    size_t n = 0;

    for (size_t i = 0; s[i] != '\n'; i++)
        n = n * 10 + s[i] - '0';

    return n;
}

Network* Load_Network(char* path)
{
    Network* n = calloc(1, sizeof(Network));
    n->sizes = new_DoubleList();
    n->biases = new_List();
    n->weights = new_List();

    //file reading line per line taken from stackoverflow
    FILE* fp;
    char* line = NULL;
    size_t len = 0;

    fp = fopen(path, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    getline(&line, &len, fp);

    n->num_layers = StrToSize_t(line);

    for (size_t i = 0; i < n->num_layers; i++)
    {
        getline(&line, &len, fp);
        append_d(n->sizes, StrToSize_t(line));
    }

    for (size_t count = 0; count < 2 * (n->num_layers - 1); count++)
    {
        //read is length of line
        //line is the char* with the text

        size_t w;
        size_t h;

        getline(&line, &len, fp);
        w = StrToSize_t(line);
        getline(&line, &len, fp);
        h = StrToSize_t(line);

        Matrix* m = new_Matrix(w, h);

        for (size_t j = 0; j < h; j++)
        {
            getline(&line, &len, fp);
            size_t width = 0;

            //line contains a whole row of digits, need to splice by spaces
            for (size_t i = 0; i < len; i++)
            {
                char val[20];
                for (size_t k = 0; k < 19; k++)
                {
                    if (line[i + k] == '\n')
                    {
                        val[k] = '\0';
                        break;
                    }
                    else
                        val[k] = line[i + k];
                }

                m->values[width][h] = strtod(val, NULL);
            }
        }

        if (count >= n->num_layers - 1)
            append(n->weights, m);
        else
            append(n->biases, m);
    }

    fclose(fp);
    if (line)
        free(line);

    return n;
}