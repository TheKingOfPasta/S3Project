# include <math.h>

/**
  * The Sigmoid function used to scale down x to a value between ]-1; 1[
  */
double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

