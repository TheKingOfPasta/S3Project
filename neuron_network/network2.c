# include "network2.h"
# include "mnist_reader.h"

void random_mat(Mat *m)
{
    for (size_t i = 0; i < m->row * m->col; ++i)
    {
        m->arr[i] = 2.f * rand() / RAND_MAX - 1.f;
        printf("%f ", m->arr[i]);
    }
    printf("\n");
}

void print_arr(float *arr, size_t n)
{
    for (size_t i = 0 ; i < n; ++i)
        printf("%f ", arr[i]);
    printf("\n");
}



void swap(void *a, void *b, size_t data_size)
{
    void *tmp = malloc(data_size);
    memcpy(tmp, a, data_size);
    memcpy(a, b, data_size);
    memcpy(b, tmp, data_size);

    free(tmp);
}

void shuffle(void* a, size_t n, size_t data_size)
{
    for (size_t i = n - 1; i > 0; --i)
    {
        size_t j = arc4random_uniform(i);

        swap((void *)(a + i * data_size), (void *)(a + j * data_size), data_size);
    }
}



void init(Network *n)
{
    for (size_t i = 0; i < NUM_LAY - 1; ++i)
    {
        random_mat(n->weights[i]);
        random_mat(n->biases[i]);
    }
}

float sigmoid(float x)
{
    return 1 / (exp(-x) + 1);
}

float sigmoid_prime(float x)
{
    float sig = sigmoid(x);
    return sig * (1 - sig);
}

void feedforward(Network *n, Mat *input, Mat *output)
{
    float a[HIDDEN_LAYER];
    for (size_t i = 0; i < HIDDEN_LAYER; ++i)
    {
        float cell = n->biases[0]->arr[i];
        for (size_t k = 0; k < INPUT_LAYER; ++k)
            cell += n->weights[0]->arr[i * INPUT_LAYER + k] *
                input->arr[k];
        a[i] = sigmoid(cell);
    }
    for (size_t i = 0; i < OUTPUT_LAYER; ++i)
    {
        float cell = n->biases[1]->arr[i];
        for (size_t k = 0; k < HIDDEN_LAYER; ++k)
            cell += n->weights[1]->arr[i * HIDDEN_LAYER + k] *
                a[k];
        output->arr[i] = sigmoid(cell);
    }
}

void backprop(Network *n, Data *data, float *nabla_w0, float *nabla_w1,
        float *nabla_b0, float *nabla_b1)
{
    float *acti0 = data->image;
    float acti1[HIDDEN_LAYER], acti2[OUTPUT_LAYER];
    float zs1[HIDDEN_LAYER], zs2[OUTPUT_LAYER];
    // feedforward
    for (size_t i = 0; i < HIDDEN_LAYER; ++i)
    {
        float cell = n->biases[0]->arr[i];
        for (size_t k = 0; k < INPUT_LAYER; ++k)
            cell += n->weights[0]->arr[i * INPUT_LAYER + k] *
                acti0[k];
        zs1[i] = cell;
        acti1[i] = sigmoid(cell);
    }
    for (size_t i = 0; i < OUTPUT_LAYER; ++i)
    {
        float cell = n->biases[1]->arr[i];
        for (size_t k = 0; k < HIDDEN_LAYER; ++k)
            cell += n->weights[1]->arr[i * HIDDEN_LAYER + k] *
                acti1[k];
        zs2[i] = cell;
        acti2[i] = sigmoid(cell);
    }
    float delta[OUTPUT_LAYER];
    float delta1[HIDDEN_LAYER];
    float dnabla_w1[OUTPUT_LAYER * HIDDEN_LAYER];
    // backward pass
    for (size_t i = 0; i < OUTPUT_LAYER; ++i)
    {
        delta[i] = (acti2[i] - data->expected[i]) * sigmoid_prime(zs2[i]);
        nabla_b1[i] += delta[i];
    }
    //print_arr(nabla_b1, OUTPUT_LAYER);
    for (size_t i = 0; i < OUTPUT_LAYER; ++i)
        for (size_t j = 0; j < HIDDEN_LAYER; ++j)
        {
            dnabla_w1[i * HIDDEN_LAYER + j] = delta[i] * acti1[j]; 
            nabla_w1[i * HIDDEN_LAYER + j] += dnabla_w1[i * HIDDEN_LAYER + j];
        }
    //print_arr(dnabla_w1, OUTPUT_LAYER * HIDDEN_LAYER);
    for (size_t i = 0; i < HIDDEN_LAYER; ++i)
    {
        delta1[i] = 0;
        for (size_t k = 0; k < OUTPUT_LAYER; ++k)
        {
            delta1[i] += dnabla_w1[i * OUTPUT_LAYER + k] * delta[k];
        }
        delta1[i] *= zs1[i];
        nabla_b0[i] += delta1[i];
    }

    for (size_t i = 0; i < HIDDEN_LAYER; ++i)
        for (size_t j = 0; j < INPUT_LAYER; ++j)
        {
            nabla_w0[i * INPUT_LAYER + j] += delta1[i] * acti0[j];
        }
}

void update_mini_batch(Network *n, Data **data, size_t k, size_t k_max,
        float eta)
{
    float nabla_w0[HIDDEN_LAYER * INPUT_LAYER] = {0,};
    float nabla_w1[OUTPUT_LAYER * HIDDEN_LAYER] = {0,};
    float nabla_b0[HIDDEN_LAYER] = {0,};
    float nabla_b1[OUTPUT_LAYER] = {0,};

    for (size_t i = k; i < k_max; ++i)
    {
        backprop(n, data[i], nabla_w0, nabla_w1, nabla_b0, nabla_b1);
    }

    float d_eta = eta / (k_max - k);
    //printf("deta %f\n", d_eta);
    //print_arr(nabla_w0, HIDDEN_LAYER * INPUT_LAYER);
    //print_arr(nabla_w1, OUTPUT_LAYER * HIDDEN_LAYER);
    //print_arr(nabla_b0, HIDDEN_LAYER);
    //print_arr(nabla_b1, OUTPUT_LAYER);



    for (size_t i = 0; i < OUTPUT_LAYER; ++i)
    {
        for (size_t j = 0; j < HIDDEN_LAYER; ++j)
        {
            n->weights[1]->arr[j * OUTPUT_LAYER + i] -= d_eta * nabla_w1[j *
                OUTPUT_LAYER + i];
        }
        n->biases[1]->arr[i] -= d_eta * nabla_b1[i];
    }
    for (size_t i = 0; i < HIDDEN_LAYER; ++i)
    {
        for (size_t j = 0; j < INPUT_LAYER; ++j)
        {
            n->weights[0]->arr[j * HIDDEN_LAYER + i] -= d_eta * nabla_w0[j *
                HIDDEN_LAYER + i];
        }
        n->biases[0]->arr[i] -= d_eta * nabla_b0[i];
    }
}

int max(float *output, float *expected, size_t n, float *cost)
{
    *cost = 0;
    int j = 0;
    int j_exp = 0;
    for (size_t i = 0; i < n; ++i)
    {
        float diff = (output[i] - expected[i]);
        *cost += diff * diff;
        if (output[i] > output[j])
           j = i;
        if (expected[i] > expected[j_exp])
            j_exp = i;
    }
    *cost /= n;
    if (OUTPUT_LAYER == 1)
        return output[0] - expected[0] < 0.5;
    return j == j_exp;
}

int evaluate(Network *net, Data **test, size_t n_test, float *cost)
{
    float out [OUTPUT_LAYER];
    Mat input = {INPUT_LAYER, 1, NULL};
    Mat output = {OUTPUT_LAYER, 1, out};
    *cost = 0;
    int good = 0;
    for (size_t i = 0; i < n_test; ++i)
    {
        input.arr = test[i]->image;
        feedforward(net, &input, &output);
        float dcost;
        good += max(output.arr, test[i]->expected, OUTPUT_LAYER, &dcost);
        *cost += dcost;
    }
    *cost /= n_test;
    return good;
}

void sgd(Network *net, Data **data, size_t n, int epochs, size_t
        mini_batch_size, float eta, Data **test, size_t n_test)
{
    for (int i = 0; i < epochs; ++i)
    {
        shuffle(data, n, sizeof(*data));
        for (size_t k = 0; k < n; k += mini_batch_size)
        {
            size_t k_max = k + mini_batch_size;
            if (k_max > n)
                 k_max = n;
            update_mini_batch(net, data, k, k_max, eta);

        }

        printf("Epoch %i completed\n", i);
        if (n_test > 0)
        {
            float cost;
            int good = evaluate(net, test, n_test, &cost);
            printf("%i / %zu | loss %f | confidence: %f\n", good, n_test, cost, 1 -
                    cost);
        }
    }
}

int main()
{
    // INIT
    float m1[HIDDEN_LAYER * INPUT_LAYER],
          m2[OUTPUT_LAYER * HIDDEN_LAYER],
          b1[HIDDEN_LAYER],
          b2[OUTPUT_LAYER];

    Mat wm1 = 
    {
        .row = HIDDEN_LAYER,
        .col = INPUT_LAYER,
        .arr = m1
    };
    Mat wm2 =
    {
        .row = OUTPUT_LAYER,
        .col = HIDDEN_LAYER,
        .arr = m2
    };
    Mat bm1 =
    {
        .row = HIDDEN_LAYER,
        .col = 1,
        .arr = b1
    };
    Mat bm2 =
    {
        .row = OUTPUT_LAYER,
        .col = 1,
        .arr = b2
    };
    Mat *biases[] = {&bm1, &bm2};
    Mat *weights[] = {&wm1, &wm2};
    Network n = 
    {
        .weights = weights,
        .biases = biases,
    };
    init(&n);

    //float inp[] = {0, 1};
    //float o[] = {0};
    //Mat input = {2, 1, inp};
    //Mat output = {1, 1, o};
    //feedforward(&n, &input, &output);
    //printf("output : %f\n", output.arr[0]);

    //float a1[] = {0, 0};
    //float a2[] = {0, 1};
    //float a3[] = {1, 0};
    //float a4[] = {1, 1};
    //float e1[] = {0};
    //float e2[] = {1};
    //float e3[] = {1};
    //float e4[] = {0};
    //Data d1 = {.image = a1, .expected = e1};
    //Data d2 = {.image = a2, .expected = e2};
    //Data d3 = {.image = a3, .expected = e3};
    //Data d4 = {.image = a4, .expected = e4};

    //Data *training[] = {&d1, &d2, &d3, &d4};
    //sgd(&n, training, 4, EPOCHS, MINI_BATCH_SIZE, ETA, training, 4);
    Data **data, **tdata;
    size_t len = load_images(TRAINING_SET_IMAGE, TRAINING_SET_LABEL, &data);
    size_t tlen = load_images(TEST_SET_IMAGE, TEST_SET_LABEL, &tdata);
    printf("Loaded %p %zu\n", data, len);

    sgd(&n, data, len, EPOCHS, MINI_BATCH_SIZE, ETA, tdata, tlen);

    free_all_training_data(data,  len);
    free_all_training_data(tdata,  tlen);



    return 0;

}
