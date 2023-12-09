#include "Neural_Network/second_network.h"

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
    Matrix* delta = Sum_product_m(sub, sig);
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
        //delta = Multiply_m(tr, delta);
        delta = Sum_product_m(mult, sp);

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

size_t evaluate(Network* n, Tuple_m* test_results, size_t len)
{
    size_t sum = 0;

    for (size_t i = 0; i < len; i++)
    {
        Matrix* ff = feedforward(n, test_results[i].x);
        size_t index = argmax_m(ff);

        if (index == argmax_m(test_results[i].y))
        {
            sum += 1;
            /*if (print_matrix_as_image(test_results[i].x))
            {
                print_m(ff);
                printf("-------------->  %zu\n", argmax_m(ff));
                print_m(test_results[i].y);
            }*/
        }

        free_m(ff);
    }

    return sum;
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
        clock_t t = clock();
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

        size_t prev = -1;
        for (size_t i = 0; i < mini_b_len; i++)
        {
            size_t count = (size_t)(100.0 * (double)i / mini_b_len);

            if (count != prev)
            {
                printf("\r");
                printf("Progress : [");
                for (size_t j = 0; j < count; j++)
                    printf("-");
                for (size_t j = count; j < 100 - 1; j++)
                    printf(" ");
                printf("] %zu %%", count);
                fflush(stdout);
                prev = count;
            }
            update_mini_batch(n, mini_batches[i], mini_batch_size, eta);
        }

        if (test_data)
        {
            size_t ev = evaluate(n, test_data, n_test);

            printf("\rEpoch %li: %li / %li = %f%%  (%fs)                                                                                                                  \n", j + 1,
                    ev, n_test, 100 * (double)ev / n_test, (double)(clock() - t)/1000000);
        }
        else
            printf("\rEpoch %li complete                                                                             \n", j);

        for (size_t i = 0; i < index; i++)
        {
            free(mini_batches[i]);
        }
        free(mini_batches);
    }
}
