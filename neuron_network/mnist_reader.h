# pragma once

# include <fcntl.h>
# include <unistd.h>
# include <err.h>
# include <stdint.h>

# define TRAINING_SET_IMAGE "./database/train-images.idx3-ubyte"
# define TRAINING_SET_LABEL "./database/train-labels.idx1-ubyte"
# define TEST_SET_IMAGE "./database/t10k-images.idx3-ubyte"
# define TEST_SET_LABEL "./database/t10k-labels.idx1-ubyte"
# define IMG_WIDTH 28
# define IMG_HEIGHT 28
# define NUMBER_DIGIT 10
# define MAX_BLACK 255.f

/**
  * From the given files, load all the dataset into the memory
  * Return the size of the dataset and update the pointer
  *
  * img_path: the path to the .idx3-ubyte file
  * label_path: the path to the .idx1-ubyte file
  * data: a pointer to hold the new array
  *
  * Remarks: The dataset can be heavy and freeing it is mandatory!!
  */
size_t load_images(char *img_path, char *label_path, Data ***data);

/**
  * Free all the dataset of size n
  */
void free_all_training_data(Data **data, size_t n);

/**
  * Print a matrix as an image:
  * When a value is more than 0, it is considering as an existing pixel,
  * otherwise an empty one.
  */
void print_image(float *a);

// ===========================================================================
// ============================================================================
// Below contains only function related to the above functions.
// You probably want to use only the above ones.
//

/**
  * Change the MSB of an int to LSB int and vice versa
  */
void flip_int(unsigned char * ptr);

/**
  * Check that the headers of the label and image files are matching the
  * expectations.
  * Otherwise throw errors
  * Returns the number of items (images)
  */
int check_header(int img_fd, int lbl_fd);


/**
  * Read the image file descriptor to read the IMG_WIDTH*IMG_HEIGHT bytes to fill the
  * matrix a of the same dimension. The byte value is put between 0 and 1
  */
void load_image(int img_fd, float *image);

/**
  * Read the label file descriptor to read the byte corresponding to what the
  * image is representing (0 to 9)
  * Fill a matrix of size (10, 1) where the index of the value is set to 1
  */
void load_label(int lbl_fd, float *expected);

/**
  * Allocate a freshly new Data struct and fill it by reading the image
  * and label file descriptors
  */
Data *load_image_label(int img_fd, int lbl_fd);

