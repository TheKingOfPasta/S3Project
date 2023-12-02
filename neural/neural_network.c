#include <stdlib.h>

typedef struct Matrix
{
    double** values;
    size_t w;
    size_t h;
} Matrix;

typedef struct Tuple
{
    Matrix* x;
    size_t y;
} Tuple;

typedef struct Network
{
    size_t num_layers;
    size_t* sizes;
    //weights and biases = list of matrix pointers
    Matrix** biases;
    Matrix** weights;
} Network;

Matrix* new_Matrix(size_t w, size_t h)
{
    Matrix* m = calloc(1, sizeof(Matrix));
    m->w = w;
    m->h = h;

    m->values = malloc(sizeof(double*) * w);
    for (size_t i = 0; i < w; i++)
        m->values[i] = calloc(h, sizeof(double));

    return m;
}

Network* new_Network(size_t* sizesArr, size_t len)
{
    Network* n = malloc(sizeof(Network));

    n->num_layers = len;
    n->sizes = malloc(sizeof(size_t) * len);
    for (size_t i = 0; i < len; i++)
        n->sizes[i] = sizesArr[i];

    n->biases = malloc((len - 1) * sizeof(Matrix*));
    for (size_t i = 0; i < len - 1; i++)
        n->biases[i] = new_Matrix(1, n->sizes[i + 1]);
    //Init biases randomly
    n->weights = malloc((len - 1) * sizeof(Matrix*));
    for (size_t i = 0; i < len - 1; i++)
        n->weights[i] = new_Matrix(n->sizes[i], n->sizes[i + 1]);
}
        //self.weights = [np.random.randn(y, x)
        //                for x, y in zip(sizes[:-1], sizes[1:])]
        //self.biases = [np.random.randn(y, 1) for y in sizes[1:]]

void Free_Matrix(Matrix* m)
{
    free(m->values);
    free(m);
}

void Free_Network(Network* n)
{
    free(n->sizes);
    for (size_t i = 0; i < 2; i++)
        Free_Matrix(n->biases[i]);
    for (size_t i = 0; i < 2; i++)
        Free_Matrix(n->weights[i]);
    free(n);
}

double sigmoid(double z)
{
    return 1.0 / (1.0 + exp(-z));
}

void sigmoid_m(Matrix* m)
{
    for (int i = 0; i < m->w; i++)
    for (int j = 0; j < m->h; j++)
        m->values[i][j] = sigmoid(m->values[i][j]);
}

double sigmoid_prime(double z)
{
    return sigmoid(z)*(1-sigmoid(z));
}

void sigmoid_prime_m(Matrix* m)
{
    for (int i = 0; i < m->w; i++)
    for (int j = 0; j < m->h; j++)
        m->values[i][j] = sigmoid_prime(m->values[i][j]);
}

void Multiply_m(Matrix* a, Matrix* b, Matrix** dest)
{
    if (a->w != b->h)
        errx(EXIT_FAILURE, "multiplying matrices with width %i"
                            " and height %i (should be equal)", a->w, b->h);

    /*if (*dest != NULL)
        free(*dest);*/

    *dest = new_Matrix(b->w, a->h);

    for (size_t i = 0; i < b->w; i++)
    for (size_t j = 0; j < a->h; j++)
    {
        (*dest)->values[i][j] = 0;
        for (size_t k = 0; k < a->w; k++)
            (*dest)->values[i][j] += a->values[k][j] * b->values[i][k];
    }
}

void Add_m(Matrix* a, Matrix* b, Matrix** dest)
{
    if (a->w != b->w || a->h != b->h)
        errx(EXIT_FAILURE, "adding matrices with sizes (%i, %i)"
                            " and (%i, %i)", a->w, a->h, b->w, b->h);

    /*if (*dest != NULL)// commented since we assume *dest is empty
        free(*dest);*/

    *dest = new_Matrix(a->w, a->h);

    for (size_t i = 0; i < a->w; i++)
    for (size_t j = 0; j < a->h; j++)
        (*dest)->values[i][j] = a->values[i][j] + b->values[i][j];
}

void Sub_m(Matrix* a, Matrix* b, Matrix** dest)
{
    if (a->w != b->w || a->h != b->h)
        errx(EXIT_FAILURE, "subtracting matrices with sizes (%i, %i)"
                            " and (%i, %i)", a->w, a->h, b->w, b->h);

    /*if (*dest != NULL)// commented since we assume *dest is empty
        free(*dest);*/

    *dest = new_Matrix(a->w, a->h);

    for (size_t i = 0; i < a->w; i++)
    for (size_t j = 0; j < a->h; j++)
        (*dest)->values[i][j] = a->values[i][j] - b->values[i][j];
}

Matrix* feedforward(Network* n, Matrix* a)
{
    // a = column matrix of input (1 * 784) ( apparently :( )
    for (size_t i = 0; i < n->num_layers - 1; i++)
    {// loop biases
        Matrix* b = n->biases[i];
        for (size_t j = 0; j < n->num_layers - 1; j++)
        {// loop weights

            Matrix* res;//Filled by Multiply_m

            Multiply_m(n->weights[j], a, &res);

            Add_m(res, b, &a);

            sigmoid_m(a);

            free(res);
        }
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

void backprop(Network* n, Matrix* x, Matrix* y, Matrix*** nabla_b, Matrix*** nabla_w)
{
    *nabla_b = malloc(sizeof(Matrix*) * (n->num_layers - 1));
    *nabla_w = malloc(sizeof(Matrix*) * (n->num_layers - 1));

    for (size_t i = 0; i < n->num_layers - 1; i++)
    {
        (*nabla_b)[i] = new_Matrix(1, n->sizes[i + 1]);
        (*nabla_w)[i] = new_Matrix(n->sizes[i], n->sizes[i + 1]);
    }

    Matrix* activation = Copy_m(x);

    Matrix** activations = malloc(sizeof(Matrix*) *
                                (1 + (n->num_layers - 1) *
                                     (n->num_layers - 1)));

    activations[0] = Copy_m(x);

    Matrix** zs = malloc(sizeof(Matrix*) *
                            (n->num_layers - 1) *
                            (n->num_layers - 1));

    size_t total = 0;
    for (size_t i = 0; i < n->num_layers - 1; i++)
    for (size_t j = 0; j < n->num_layers - 1; j++)
    {
        Multiply_m(n->weights[j], activation, &(zs[total]));
        Add_m(zs[total], n->biases[i], &(zs[total]));

        free(activation);
        activation = Copy_m(zs[total]);
        sigmoid_m(activation);
        activations[total + 1] = Copy_m(activation);

        total += 1;
    }

    Sub_m(activations[(n->num_layers - 1) * (n->num_layers - 1)], y, &nabla_b[n->num_layers - 2]);
    Matrix* cpy = Copy_m(zs[(n->num_layers - 1) * (n->num_layers - 1) - 1]);
    sigmoid_prime_m(cpy);
    Multiply_m(nabla_b[n->num_layers - 2], cpy, &nabla_b[n->num_layers - 2]);

    Multiply_m(nabla_b[n->num_layers - 2],
            Transpose_m(activations[(n->num_layers - 1) * (n->num_layers - 1) - 1]),
            &nabla_w[n->num_layers - 2]);

    Matrix* delta = nabla_b[n->num_layers - 2];

    for (size_t l = 2; l < n->num_layers; l++)
    {
        Matrix* sp = Copy_m(zs[-l]);
        sigmoid_prime_m(sp);
        Multiply_m(Transpose_m(n->weights[n->num_layers - l]), delta, &delta);
        Multiply_m(delta, sp, &delta);
        nabla_b[-l] = Copy_m(delta);
        Multiply_m(
                delta,
                Transpose_m(activations[(1 + (n->num_layers - 1) *
                                     (n->num_layers - 1)) - l - 1]),
                &nabla_w[(n->num_layers - 1)-l]);

        /*z = zs[-l]
        sp = sigmoid_prime(z)
        delta = np.dot(self.weights[-l+1].transpose(), delta) * sp
        nabla_b[-l] = delta
        nabla_w[-l] = np.dot(delta, activations[-l-1].transpose())*/
    }
}

/*    def SGD(self, training_data, epochs, mini_batch_size, eta, test_data=None):
        Train the neural network using mini-batch stochastic
        gradient descent.  The ``training_data`` is a list of tuples
        ``(x, y)`` representing the training inputs and the desired
        outputs.  The other non-optional parameters are
        self-explanatory.  If ``test_data`` is provided then the
        network will be evaluated against the test data after each
        epoch, and partial progress printed out.  This is useful for
        tracking progress, but slows things down substantially.

        if test_data: n_test = len(test_data)
        n = len(training_data)
        for j in range(epochs):
            random.shuffle(training_data)
            mini_batches = [
                training_data[k:k+mini_batch_size]
                for k in range(0, n, mini_batch_size)]
            for mini_batch in mini_batches:
                self.update_mini_batch(mini_batch, eta)
            if test_data:
                print "Epoch {0}: {1} / {2}".format(
                    j, self.evaluate(test_data), n_test)
            else:
                print "Epoch {0} complete".format(j)

    def update_mini_batch(self, mini_batch, eta):
        Update the network's weights and biases by applying
        gradient descent using backpropagation to a single mini batch.
        The ``mini_batch`` is a list of tuples ``(x, y)``, and ``eta``
        is the learning rate.
        nabla_b = [np.zeros(b.shape) for b in self.biases]
        nabla_w = [np.zeros(w.shape) for w in self.weights]
        for x, y in mini_batch:
            delta_nabla_b, delta_nabla_w = self.backprop(x, y)
            nabla_b = [nb+dnb for nb, dnb in zip(nabla_b, delta_nabla_b)]
            nabla_w = [nw+dnw for nw, dnw in zip(nabla_w, delta_nabla_w)]
        self.weights = [w-(eta/len(mini_batch))*nw
                        for w, nw in zip(self.weights, nabla_w)]
        self.biases = [b-(eta/len(mini_batch))*nb
                       for b, nb in zip(self.biases, nabla_b)];*/

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

size_t evaluate(Network* n, Tuple* test_results, size_t len)
{
    size_t sum = 0;

    for (size_t i = 0; i < len; i++)
    {
        if (argmax_m(feedforward(n, test_results[i].x)) == test_results[i].y)
            sum += 1;
    }

    return sum;
}