#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <math.h>

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

typedef struct Node_m
{
    struct Node_m* next;
    Matrix* m;
} Node_m;

typedef struct List_m
{
    Node_m* head;
    Node_m* tail;
    size_t len;
} List_m;

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

typedef struct ListTup
{
    List_m* x;
    List_m* y;
} ListTup;

/// @brief Subtracts d (double) to all the values of the matrix m, puts the res in *res
Matrix* Mul_m_d(Matrix* m, double d);

/// @brief Prints a column matrix 1x784 as an image in the terminal
char print_matrix_as_image(Matrix *a);

void print_m(Matrix* m);

/// @brief Don't forget to free the matrix after! (free_m)
Matrix* new_Matrix(size_t w, size_t h);

Matrix* new_Random_Matrix(size_t w, size_t h);

void Free_Matrix_List(List_m* l);

void Free_List(DoubleList* l);

void append(List_m* l, Matrix* v);

void append_d(DoubleList* l, size_t v);

Node_m* atIndex(List_m* l, ssize_t i);

DoubleNode* atIndex_d(DoubleList* l, ssize_t i);

DoubleList* new_DoubleList();

List_m* new_List();

void print_list(DoubleList* l);

void print_list_m(List_m* l);

void free_m(Matrix* m);

Matrix* Multiply_m(Matrix* a, Matrix* b);

Matrix* Add_m(Matrix* a, Matrix* b);

Matrix* Sub_m(Matrix* a, Matrix* b);

Matrix* Copy_m(Matrix* m);

Matrix* Transpose_m(Matrix* m);

Matrix* Sum_product_m(Matrix* a, Matrix* b);

void shuffle(Tuple_m* arr, size_t len);

size_t argmax_m(Matrix* m);
