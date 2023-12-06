#pragma once

#include <stdlib.h>
#include <math.h>
#include <err.h>
#include <stdio.h>
#include <sys/types.h>

typedef struct Matrix
{
    double** values;
    size_t w;
    size_t h;
} Matrix;

typedef struct Tuple_m
{
    Matrix* x;
    Matrix* y;
} Tuple_m;

typedef struct Tuple
{
    Matrix* x;
    size_t y;
} Tuple;

typedef struct Node
{
    struct Node* next;
    Matrix* m;
} Node;

typedef struct List
{
    Node* head;
    Node* tail;
    size_t len;
} List;

typedef struct DoubleNode
{
    struct DoubleNode* next;
    size_t d;
} DoubleNode;

typedef struct DoubleList
{
    DoubleNode* head;
    DoubleNode* tail;
    size_t len;
} DoubleList;

typedef struct Network
{
    size_t num_layers;
    DoubleList* sizes;
    //weights and biases = list of matrix pointers
    List* biases;
    List* weights;
} Network;

typedef struct ListTup
{
    List* x;
    List* y;
} ListTup;

void print_list_m(List* l);

void Free_List(DoubleList* l);

Node* atIndex(List* l, ssize_t i);

void print_m(Matrix* m);

Matrix* new_Matrix(size_t w, size_t h);

Matrix* new_Random_Matrix(size_t w, size_t h);

Network* new_Network(size_t* sizesArr, size_t len);

void free_m(Matrix* m);

void Free_Network(Network* n);

double sigmoid(double z);

Matrix* sigmoid_m(Matrix* m);

double sigmoid_prime(double z);

Matrix* sigmoid_prime_m(Matrix* m);

Matrix* kysnumpy(Matrix* a, Matrix* b);

Matrix* Multiply_m(Matrix* a, Matrix* b);

Matrix* Add_m(Matrix* a, Matrix* b);

Matrix* Sub_m(Matrix* a, Matrix* b);

Matrix* feedforward(Network* n, Matrix* a);

Matrix* Copy_m(Matrix* m);

Matrix* Transpose_m(Matrix* m);

ListTup* backprop(Network* n, Matrix* x, Matrix* y);

void shuffle(Tuple_m* arr, size_t len);

size_t argmax_m(Matrix* m);

size_t evaluate(Network* n, Tuple_m* test_results, size_t len);

Matrix* Mul_m_d(Matrix* m, double d);

void update_mini_batch(Network* n, Tuple_m* mini_batch, size_t len, double eta);

void SGD(Network* n,
         Tuple_m* training_data,
         size_t n_training,
         size_t epochs,
         size_t mini_batch_size,
         double eta,
         Tuple_m* test_data,
         size_t n_test);