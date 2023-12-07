#include "Utils/matrix.h"

Matrix* Mul_m_d(Matrix* m, double d)
{
    Matrix* res = new_Matrix(m->w, m->h);

    for (size_t i = 0; i < m->w; i++)
    for (size_t j = 0; j < m->h; j++)
        res->values[i][j] = m->values[i][j] * d;

    return res;
}

char print_matrix_as_image(Matrix *a)
{
    char f = 0;

    for (size_t i = 0; i < a->w; i++)
    for (size_t j = 0; j < a->h; j++)
        if (a->values[i][j] != 0)
        {
            f = 1;
            break;
        }
    if (!f)
        return 0;

    for (size_t i = 0; i < 28; ++i)
    {
        for (size_t j = 0; j < 28; ++j)
            if (a->values[0][i * 28 + j])
            {
                printf("@ ");
            }
            else
            {
                printf(". ");
            }
        printf("\n");
    }

    return 1;
}


void print_m(Matrix* m)
{
    printf("--");
    for (size_t i = 0; i < m->w; i++)
        printf("-------");
    printf("--\n");
    for (size_t j = 0; j < m->h; j++)
    {
        printf("| ");
        for (size_t i = 0; i < m->w; i++)
            printf("%.04f ", m->values[i][j]);
        printf(" |\n");
    }
    printf("--");
    for (size_t i = 0; i < m->w; i++)
        printf("-------");
    printf("--\n");
}

Matrix* new_Matrix(size_t w, size_t h)
{
    Matrix* m = calloc(1, sizeof(Matrix));
    if (!m)
        errx(EXIT_FAILURE, "Failed to malloc matrix");
    m->w = w;
    m->h = h;

    m->values = malloc(sizeof(double*) * w);
    if (!(m->values))
        errx(EXIT_FAILURE, "Failed to malloc matrix columns");
    for (size_t i = 0; i < w; i++)
    {
        m->values[i] = calloc(h, sizeof(double));
        if (!(m->values[i]))
            errx(EXIT_FAILURE, "Failed to malloc matrix row");
    }

    return m;
}

Matrix* new_Random_Matrix(size_t w, size_t h)
{
    Matrix* m = new_Matrix(w, h);

    for (size_t i = 0; i < w; i++)
    for (size_t j = 0; j < h; j++)
    {
        m->values[i][j] = (rand() / (double)RAND_MAX) / sqrt(w);
    }

    return m;
}

void Free_Matrix_List(List_m* l)
{
    for (Node_m* n = l->head; n;)
    {
        free_m(n->m);
        Node_m* curr = n;
        n = n->next;
        free(curr);
    }

    free(l);
}

void Free_List(DoubleList* l)
{
    for (DoubleNode* n = l->head; n;)
    {
        DoubleNode* curr = n;
        n = n->next;
        free(curr);
    }

    free(l);
}

void append(List_m* l, Matrix* v)
{
    Node_m* new = malloc(sizeof(Node_m));
    new->m = v;
    new->next = NULL;
    if (l->tail)
        l->tail->next = new;

    l->tail = new;

    if (!(l->head))
        l->head = new;

    l->len += 1;
}

void append_d(DoubleList* l, size_t v)
{
    DoubleNode* new = malloc(sizeof(DoubleNode));
    new->d = v;
    new->next = NULL;
    if (l->tail)
        l->tail->next = new;

    l->tail = new;

    if (!(l->head))
        l->head = new;

    l->len += 1;
}

Node_m* atIndex(List_m* l, ssize_t i)
{
    while (i < 0)
        i += l->len;

    Node_m* n = l->head;
    while (n && i)
    {
        n = n->next;
        i -= 1;
    }

    if (!n)
        printf("Out of bounds error\n");

    return n;
}

DoubleNode* atIndex_d(DoubleList* l, ssize_t i)
{
    while (i < 0)
        i += l->len;

    DoubleNode* n = l->head;
    while (n && i)
    {
        n = n->next;
        i -= 1;
    }

    if (!n)
        printf("Out of bounds error\n");

    return n;
}

DoubleList* new_DoubleList()
{
    DoubleList* l = malloc(sizeof(DoubleList));
    l->head = NULL;
    l->tail = NULL;
    l->len = 0;
    return l;
}

List_m* new_List()
{
    List_m* l = malloc(sizeof(List_m));
    l->head = NULL;
    l->tail = NULL;
    l->len = 0;
    return l;
}

void print_list(DoubleList* l)
{
    for (DoubleNode* n = l->head; n; n = n->next)
        printf("%li ", n->d);
    printf("\n");
}

void print_list_m(List_m* l)
{
    for (Node_m* n = l->head; n; n = n->next)
    {
        print_m(n->m);
        printf("\n\n---------------------------------------------------------\n\n");
    }
}

void free_m(Matrix* m)
{
    for (size_t i = 0; i < m->w; i++)
        free(m->values[i]);
    free(m->values);
    free(m);
}

Matrix* Multiply_m(Matrix* a, Matrix* b)
{
    if (a->w != b->h)
    {
        printf("Multiplying matrices with incorrect w/h (%lix%li  %lix%li)\n", a->w, a->h, b->w, b->h);
        free_m(a);
        free_m(b);
    }

    Matrix* res = new_Matrix(b->w, a->h);

    for (size_t i = 0; i < b->w; i++)
    for (size_t j = 0; j < a->h; j++)
    {
        res->values[i][j] = 0;
        for (size_t k = 0; k < a->w; k++)
            res->values[i][j] += a->values[k][j] * b->values[i][k];
    }

    return res;
}

Matrix* Add_m(Matrix* a, Matrix* b)
{
    if (a->w != b->w || a->h != b->h)
    {
        printf("Adding matrices with incorrect w/h (%lix%li  %lix%li)\n", a->w, a->h, b->w, b->h);
        print_m(a);
        print_m(b);
        free_m(a);
        free_m(b);
    }

    Matrix* res = new_Matrix(a->w, a->h);

    for (size_t i = 0; i < a->w; i++)
    for (size_t j = 0; j < a->h; j++)
        res->values[i][j] = a->values[i][j] + b->values[i][j];

    return res;
}

Matrix* Sub_m(Matrix* a, Matrix* b)
{
    if (a->w != b->w || a->h != b->h)
    {
        printf("Subtracting matrices with incorrect w/h (%lix%li  %lix%li)\n", a->w, a->h, b->w, b->h);
        print_m(a);
        print_m(b);
        free_m(a);
        free_m(b);
    }

    Matrix* res = new_Matrix(a->w, a->h);

    for (size_t i = 0; i < a->w; i++)
    for (size_t j = 0; j < a->h; j++)
        res->values[i][j] = a->values[i][j] - b->values[i][j];

    return res;
}

Matrix* Copy_m(Matrix* m)
{
    Matrix* res = new_Matrix(m->w, m->h);

    for (size_t i = 0; i < m->w; i++)
    for (size_t j = 0; j < m->h; j++)
        res->values[i][j] = m->values[i][j];

    return res;
}

Matrix* Transpose_m(Matrix* m)
{
    Matrix* res = new_Matrix(m->h, m->w);

    for (size_t i = 0; i < m->w; i++)
    for (size_t j = 0; j < m->h; j++)
        res->values[j][i] = m->values[i][j];

    return res;
}

Matrix* Sum_product_m(Matrix* a, Matrix* b)
{
    Matrix* res = new_Matrix(a->w, a->h);

    for (size_t i = 0; i < a->w; i++)
        for (size_t j = 0; j < a->h; j++)
            res->values[i][j] = a->values[i][j] * b->values[i][j];

    return res;
}

void shuffle(Tuple_m* arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        size_t randomI = rand() % len;

        Tuple_m temp = arr[randomI];
        arr[randomI] = arr[len - 1 - randomI];
        arr[len - 1 - randomI] = temp;
    }
}

size_t argmax_m(Matrix* m)
{
    size_t vX = 0;
    size_t vY = 0;

    for (size_t i = 0; i < m->w; i++)
    for (size_t j = 0; j < m->h; j++)
        if (m->values[i][j] > m->values[vX][vY])
        {
            vX = i;
            vY = j;
        }

    return vX + vY * m->w;
}
