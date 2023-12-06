#include "second_network.h"

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
        m->values[i][j] = (rand() / (double)RAND_MAX - 0.5);
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

/*List_m* splice(List_m* l, ssize_t start, ssize_t end)
{
    List_m* res = new_List();

    while (start < 0)
        start += l->len;

    while (end <= 0)
        end += l->len;

    res->len = end - start;

    end -= 1;//end is exclusive

    Node_m* n = atIndex(l, start);

    res->head = NULL;
    Node_m* curr = NULL;
    if (n)
    {
        res->head = malloc(sizeof(Node_m));
        res->head->m = Copy_m(n->m);
        for (curr = res->head; end > start; curr = curr->next)
        {
            curr->next = malloc(sizeof(Node_m));
            curr->next->m = Copy_m(n->m);

            n = n->next;

            end -= 1;
        }
    }
    res->tail = curr;
    curr->next = NULL;


    return res;
}*/

void print_list(DoubleList* l)
{
    for (DoubleNode* n = l->head; n; n = n->next)
        printf("%li ", n->d);
    printf("\n");
}

/*DoubleList* splice_d(DoubleList* l, ssize_t start, ssize_t end)
{
    DoubleList* res = new_DoubleList();

    while (start < 0)
        start += l->len;

    while (end <= 0)
        end += l->len;

    res->len = end - start;

    end -= 1;//end is exclusive

    DoubleNode* n = atIndex_d(l, start);

    res->head = NULL;
    DoubleNode* curr = NULL;
    if (n)
    {
        res->head = malloc(sizeof(DoubleNode));
        res->head->d = n->d;
        for (curr = res->head; end > start; curr = curr->next)
        {
            n = n->next;

            curr->next = malloc(sizeof(DoubleNode));
            curr->next->d = n->d;

            end -= 1;
        }
    }
    res->tail = curr;
    curr->next = NULL;

    return res;
}*/

void print_list_m(List_m* l)
{
    for (Node_m* n = l->head; n; n = n->next)
    {
        print_m(n->m);
        printf("\n\n---------------------------------------------------------\n\n");
    }
}

Network* new_Network(size_t* sizesArr, size_t len)
{
    Network* n = malloc(sizeof(Network));

    n->num_layers = len;
    n->sizes = new_DoubleList();
    for (size_t i = 0; i < len; i++)
        append_d(n->sizes, sizesArr[i]);

    n->biases = new_List();
    for (DoubleNode* curr = n->sizes->head->next; curr; curr = curr->next)
    {
        append(n->biases, new_Matrix(1, curr->d));
    }

    n->weights = new_List();
    for (DoubleNode *y = n->sizes->head; y->next; y = y->next)
        append(n->weights, new_Random_Matrix(y->d, y->next->d));

    return n;
}

void free_m(Matrix* m)
{
    for (size_t i = 0; i < m->w; i++)
        free(m->values[i]);
    free(m->values);
    free(m);
}

void Free_Network(Network* n)
{
    Free_List(n->sizes);
    Free_Matrix_List(n->biases);
    Free_Matrix_List(n->weights);
    free(n);
}

double sigmoid(double z)
{
    return 1.0 / (1.0 + exp(-z));
}

Matrix* sigmoid_m(Matrix* m)
{
    Matrix* res = new_Matrix(m->w, m->h);
    for (size_t i = 0; i < m->w; i++)
    for (size_t j = 0; j < m->h; j++)
        res->values[i][j] = sigmoid(m->values[i][j]);

    return res;
}

double sigmoid_prime(double z)
{
    return sigmoid(z)*(1-sigmoid(z));
}

Matrix* sigmoid_prime_m(Matrix* m)
{
    Matrix* res = new_Matrix(m->w, m->h);
    for (size_t i = 0; i < m->w; i++)
    for (size_t j = 0; j < m->h; j++)
        res->values[i][j] = sigmoid_prime(m->values[i][j]);

    return res;
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

//for b, w in zip(self.biases, self.weights):
//  a = sigmoid(np.dot(w, a)+b)
Matrix* feedforward(Network* n, Matrix* a)
{
    // a = column matrix of input (1 * 784)  apparently :(

    char f = 0;

    for (Node_m *b = n->biases->head, *w = n->weights->head; b && w; b = b->next, w = w->next)
    {
        Matrix* mult = Multiply_m(w->m, a);
        Matrix* add = Add_m(mult, b->m);

        if (f)
            free_m(a);
        else
            f = 1;

        a = sigmoid_m(add);

        free_m(add);
        free_m(mult);
    }

    return a;
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

Matrix* kysnumpy(Matrix* a, Matrix* b)
{
    Matrix* res = new_Matrix(a->w, a->h);

    for (size_t i = 0; i < a->w; i++)
        for (size_t j = 0; j < a->h; j++)
            res->values[i][j] = a->values[i][j] * b->values[i][j];

    return res;
}

ListTup* backprop(Network* n, Matrix* x, Matrix* y)
{
    List_m* nabla_b = new_List();
    List_m* nabla_w = new_List();

    for (Node_m *b = n->biases->head, *w = n->weights->head; b && w; b = b->next, w = w->next)
    {
        Matrix* b_m = b->m;
        append(nabla_b, new_Matrix(b_m->w, b_m->h));
        Matrix* w_m = w->m;
        append(nabla_w, new_Matrix(w_m->w, w_m->h));
    }

    Matrix* activation = Copy_m(x);

    List_m* activations = new_List();
    append(activations, activation);

    List_m* zs = new_List();

    for (Node_m *b = n->biases->head, *w = n->weights->head; b && w; b = b->next, w = w->next)
    {
        Matrix* mult = Multiply_m(w->m, activation);

        Matrix* z = Add_m(mult, b->m);
        append(zs, z);
        activation = sigmoid_m(z);
        append(activations, activation);

        free_m(mult);
    }

    Matrix* sub = Sub_m(atIndex(activations, -1)->m, y);
    Matrix* sig = sigmoid_prime_m(atIndex(zs, -1)->m);
    Matrix* delta = kysnumpy(sub, sig);
    free_m(atIndex(nabla_b, -1)->m);
    atIndex(nabla_b, -1)->m = delta;

    Matrix* tr = Transpose_m(atIndex(activations, -2)->m);

    free_m(atIndex(nabla_w, -1)->m);
    atIndex(nabla_w, -1)->m = Multiply_m(delta, tr);

    free_m(tr);
    free_m(sub);
    free_m(sig);

    for (size_t l = 2; l < n->num_layers; l++)
    {
        Matrix* z = atIndex(zs, -l)->m;
        Matrix* sp = sigmoid_prime_m(z);

        tr = Transpose_m(atIndex(n->weights, -l+1)->m);

        Matrix* mult = Multiply_m(tr, delta);
        delta = kysnumpy(mult, sp);

        free_m(atIndex(nabla_b, -l)->m);

        atIndex(nabla_b, -l)->m = delta;
        Matrix* temp = Transpose_m(atIndex(activations, -l-1)->m);
        free_m(atIndex(nabla_w, -l)->m);
        atIndex(nabla_w, -l)->m = Multiply_m(delta, temp);

        free_m(sp);
        free_m(temp);
        free_m(mult);
        free_m(tr);
    }

    ListTup* tup = malloc(sizeof(ListTup));
    tup->x = nabla_b;
    tup->y = nabla_w;

    Free_Matrix_List(activations);
    Free_Matrix_List(zs);

    return tup;
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

char print_img_(Matrix *a)
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

size_t evaluate(Network* n, Tuple_m* test_results, size_t len)
{
    size_t sum = 0;

    for (size_t i = 0; i < len; i++)
    {
        Matrix* ff = feedforward(n, test_results[i].x);

        if (0 && print_img_(test_results[i].x))
        {
            print_m(ff);
            printf("-------------->  %zu\n", argmax_m(ff));
        }

        if (argmax_m(ff) == test_results[i].y->values[0][0])
            sum += 1;

        free_m(ff);
    }

    return sum;
}

/// @brief Subtracts d (double) to all the values of the matrix m, puts the res in *res
Matrix* Mul_m_d(Matrix* m, double d)
{
    Matrix* res = new_Matrix(m->w, m->h);

    for (size_t i = 0; i < m->w; i++)
    for (size_t j = 0; j < m->h; j++)
        res->values[i][j] = m->values[i][j] * d;

    return res;
}

void update_mini_batch(Network* n, Tuple_m* mini_batch, size_t len, double eta)
{
    List_m* nabla_b = new_List();
    List_m* nabla_w = new_List();

    for (Node_m *b = n->biases->head, *w = n->weights->head; b && w; b = b->next, w = w->next)
    {
        Matrix* b_m = b->m;
        append(nabla_b, new_Matrix(b_m->w, b_m->h));
        Matrix* w_m = w->m;
        append(nabla_w, new_Matrix(w_m->w, w_m->h));
    }

    for (size_t i = 0; i < len; i++)
    {
        Matrix* x = mini_batch[i].x;
        Matrix* y = mini_batch[i].y;

        ListTup* tup = backprop(n, x, y);
        List_m* delta_nabla_b = tup->x;
        List_m* delta_nabla_w = tup->y;
        free(tup);

        Node_m* nb = nabla_b->head;
        for (Node_m* dnb = delta_nabla_b->head; dnb && nb; dnb = dnb->next, nb = nb->next)
        {
            Matrix* add = Add_m(nb->m, dnb->m);
            free_m(nb->m);
            nb->m = add;
        }

        Node_m* nw = nabla_w->head;
        for (Node_m* dnw = delta_nabla_w->head; dnw && nw; dnw = dnw->next, nw = nw->next)
        {
            Matrix* add = Add_m(nw->m, dnw->m);
            free_m(nw->m);
            nw->m = add;
        }

        Free_Matrix_List(delta_nabla_b);
        Free_Matrix_List(delta_nabla_w);
    }

    double frac = eta / len;

    Node_m* w = n->weights->head;
    for (Node_m* nw = nabla_w->head; nw && w; nw = nw->next, w = w->next)
    {
        Matrix* mult = Mul_m_d(nw->m, frac);
        Matrix* sub = Sub_m(w->m, mult);
        free_m(w->m);
        w->m = sub;
        free_m(mult);
    }

    Node_m* b = n->biases->head;
    for (Node_m* nb = nabla_b->head; nb && b; nb = nb->next, b = b->next)
    {
        Matrix* mult = Mul_m_d(nb->m, frac);
        Matrix* sub = Sub_m(b->m, mult);
        free_m(b->m);
        b->m = sub;
        free_m(mult);
    }

    Free_Matrix_List(nabla_b);
    Free_Matrix_List(nabla_w);
}

void SGD(Network* n,
         Tuple_m* training_data,
         size_t n_training,
         size_t epochs,
         size_t mini_batch_size,
         double eta,
         Tuple_m* test_data,
         size_t n_test)
{
    size_t mini_b_len = n_training / mini_batch_size;
    for (size_t j = 0; j < epochs; j++)
    {
        //print_list_m(n->biases);
        //print_list_m(n->weights);
        shuffle(training_data, n_training);
        Tuple_m** mini_batches = malloc(sizeof(Tuple_m*) * mini_b_len);
        size_t index = 0;

        for (size_t k = 0; k < n_training; k += mini_batch_size)
        {
            mini_batches[index] = malloc(sizeof(Tuple_m) * mini_batch_size);
            for (size_t i = 0; i < mini_batch_size; i++)
            {
                mini_batches[index][i] = training_data[k + i];
            }

            index += 1;
        }

        for (size_t i = 0; i < mini_b_len; i++)
        {
            update_mini_batch(n, mini_batches[i], mini_batch_size, eta);
        }

        if (0)
        if (test_data)
        {
            size_t ev = evaluate(n, test_data, n_test);
            printf("Epoch %li: %li / %li = %f%%\n", j,
                    ev, n_test, 100 * (double)ev / n_test);
        }
        else
            printf("Epoch %li complete\n", j);

        for (size_t i = 0; i < index; i++)
        {
            free(mini_batches[i]);
        }
        free(mini_batches);
    }
}