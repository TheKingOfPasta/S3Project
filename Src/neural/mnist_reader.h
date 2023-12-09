#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// set appropriate path for data
#define TRAIN_IMAGE "database/train-images.idx3-ubyte"
#define TRAIN_LABEL "database/train-labels.idx1-ubyte"
#define TEST_IMAGE "database/t10k-images.idx3-ubyte"
#define TEST_LABEL "database/t10k-labels.idx1-ubyte"

#define LEN_INFO_IMAGE 4
#define LEN_INFO_LABEL 2

#define MAX_IMAGESIZE 1280
#define MAX_NUM_OF_IMAGES 1

unsigned char image[MAX_NUM_OF_IMAGES][MAX_IMAGESIZE][MAX_IMAGESIZE];
int width[MAX_NUM_OF_IMAGES], height[MAX_NUM_OF_IMAGES];

int info_image[LEN_INFO_IMAGE];
int info_label[LEN_INFO_LABEL];

unsigned char train_image_char[60000][784];
unsigned char test_image_char[10000][784];
unsigned char train_label_char[60000][1];
unsigned char test_label_char[10000][1];

double train_image[60000][784];
double test_image[10000][784];
int  train_label[60000];
int test_label[10000];

void Flip(unsigned char * ptr)
{
    register unsigned char val;
    // Swap 1st and 4th bytes
    val = *(ptr);
    *(ptr) = *(ptr+3);
    *(ptr+3) = val;
    // Swap 2nd and 3rd bytes
    ptr += 1;
    val = *(ptr);
    *(ptr) = *(ptr+1);
    *(ptr+1) = val;
}

void read_chars(
    char *file_path,
    int num_data,
    int len_info,
    int arr_n,
    unsigned char data_char[][arr_n],
    int info_arr[])
{
    int i;
    int fd;
    unsigned char *ptr;
    if ((fd = open(file_path, O_RDONLY)) == -1) {
        fprintf(stderr, "couldn't open image file");
        exit(-1);
    }
    read(fd, info_arr, len_info * sizeof(int));
    for (i = 0; i < len_info; i++) {
        ptr = (unsigned char *)(info_arr + i);
        Flip(ptr);
        ptr = ptr + sizeof(int);
    }
    for (i = 0; i < num_data; i++) {
        read(fd, data_char[i], arr_n * sizeof(unsigned char));
    }
    close(fd);
}

void img_ctod(int num_data, unsigned char data_image_char[][784],
              double data_image[][784])
{
    int i;
    int j;
    for (i = 0; i < num_data; i++)
        for (j = 0; j < 784; j++)
            data_image[i][j]  = (double)data_image_char[i][j] / 255.0;
}

void label_ctoi(int num_data, unsigned char data_label_char[][1],
                int data_label[])
{
    int i;
    for (i = 0; i < num_data; i++)
        data_label[i]  = (int)data_label_char[i][0];
}

void load_mnist()
{
    read_chars(TRAIN_IMAGE, 60000, LEN_INFO_IMAGE, 784, train_image_char,
               info_image);
    img_ctod(60000, train_image_char, train_image);
    read_chars(TEST_IMAGE, 10000, LEN_INFO_IMAGE, 784, test_image_char, info_image);
    img_ctod(10000, test_image_char, test_image);
    read_chars(TRAIN_LABEL, 60000, LEN_INFO_LABEL, 1, train_label_char, info_label);
    label_ctoi(60000, train_label_char, train_label);
    read_chars(TEST_LABEL, 10000, LEN_INFO_LABEL, 1, test_label_char, info_label);
    label_ctoi(10000, test_label_char, test_label);
}