# include <stdlib.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include "helper.h"
# include "neuron.h"


/**
  * Neurons and weights are created with malloc and MUST be freed before being
  * discarded
  */
void NN_free_neuron(NN_Neuron *neuron)
{
    free(neuron->weights);
    free(neuron);
}


/**
  * Create and allocate the memory needed for a neuron
  * Then fill it with random values (weight and bias)
  * Returns a pointer to the creater neuron
  *
  * Don't forget to use NN_free_neuron(NN_Neuron *neuron) to clear your work!!!
  */
NN_Neuron* NN_create_neuron(int num_inputs)
{
    NN_Neuron *neuron = (NN_Neuron *)malloc(sizeof(NN_Neuron));
    if (neuron == NULL)
        errx(EXIT_FAILURE, "Failed to allocate memory for neuron");

    neuron->num_inputs = num_inputs;
    neuron->weights = (double *)malloc(num_inputs * sizeof(double));
    if (neuron->weights == NULL)
    {
        free(neuron);
        errx(EXIT_FAILURE, "Failed to allocate memory for the weights of"
                "neurons");

    }
    // neutron memory init done
    // now filling it with random values

    for (size_t i = 0; i < num_inputs; ++i)
        neuron->weights[i] = random_value(); // subject to change
    neuron->bias = random_value(); // same

    return neuron;
}

void NN_print_neuron(NN_Neuron *neuron)
{
    if (neuron == NULL) // should it crash instead?
        return;
    printf("Neuron:\n"
            "   - num_inputs = %zi\n"
            "   - bias = %f\n"
            "   - weights = { ", neuron->num_inputs, neuron->bias);
    for (size_t i = 0; i < neuron->num_inputs; ++i)
    {
        printf("%f ", neuron->weights[i]);
    }
    printf("}\n");
}

double sigmoid_of_neuron(const double *inputs, const NN_Neuron *neuron, size_t n)
{
    return sigmoid(inputs, n, neuron->weights, neuron->bias);
}