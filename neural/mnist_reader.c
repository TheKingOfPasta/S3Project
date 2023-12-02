# include <fcntl.h>
# include <unistd.h>
# include <err.h>
# include <stdint.h>

# include "helper.h"
# include "mnist_reader.h"

/**
  * Change the MSB of an int to LSB int and vice versa
  */
void flip_int(unsigned char * ptr)
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

/**
  * Check that the headers of the label and image files are matching the
  * expectations.
  * Otherwise throw errors
  * Returns the number of items (images)
  */
int check_header(int img_fd, int lbl_fd)
{
    uint32_t img_magic, img_nb_items, lbl_magic, lbl_nb_items,
             img_width, img_height;
    read(img_fd, &img_magic, sizeof(img_magic));
    read(lbl_fd, &lbl_magic, sizeof(lbl_magic));
    read(img_fd, &img_nb_items, sizeof(img_nb_items));
    read(lbl_fd, &lbl_nb_items, sizeof(lbl_nb_items));
    // flip because these are MSB (while here its LSB)
    flip_int((unsigned char *)(&img_magic));
    flip_int((unsigned char *)(&img_nb_items));
    flip_int((unsigned char *)(&lbl_magic));
    flip_int((unsigned char *)(&lbl_nb_items));

    // reading dimensions
    read(img_fd, &img_width, sizeof(img_width));
    read(img_fd, &img_height, sizeof(img_height));
    flip_int((unsigned char *)(&img_width));
    flip_int((unsigned char *)(&img_height));
    if (img_width != IMG_WIDTH || img_height != IMG_HEIGHT)
        errx(EXIT_FAILURE, "Expected Image dimension of (%i x %i) but read (%i "
            "x %i)", IMG_WIDTH, IMG_HEIGHT, img_width, img_height);

    if (img_magic == 2049)
        errx(EXIT_FAILURE, "Training set image magic number is 2049 (label"
            "file's one), expected 2051.");
    if (img_magic != 2051)
        errx(EXIT_FAILURE, "Wrong magic number, expected 2051, received %i",
                img_magic);
    if (lbl_magic == 2051)
        errx(EXIT_FAILURE, "Label set image magic number is 2051 (training"
            "file's one), expected 2049.");
    if (lbl_magic != 2049)
        errx(EXIT_FAILURE, "Wrong magic number, expected 2049, received %i",
                lbl_magic);
    if (img_nb_items != lbl_nb_items)
        errx(EXIT_FAILURE, "Label and Training set does not contains the same"
                "amount of data. (%i against %i", lbl_nb_items, img_nb_items);
    return img_nb_items;
}

/**
  * Print a linear matrix as an image:
  * When a value is more than 0, it is considering as an existing pixel,
  * otherwise an empty one.
  */
void print_image(Matrix *a)
{
    for (size_t i = 0; i < IMG_WIDTH; ++i)
    {
        for (size_t j = 0; j < IMG_HEIGHT; ++j)
            if (a->matrix[i * IMG_HEIGHT + j][0] > 0)
                printf("@ ");
            else
                printf(". ");
        printf("\n");
    }
}

/**
  * Read the image file descriptor to read the IMG_WIDTH*IMG_HEIGHT bytes to fill the
  * matrix a of size (IMG_WIDTH*IMG_HEIGHTx1). The byte value is put between 0 and 1
  */
void load_image(int img_fd, Matrix *image)
{
    char buffer[IMG_WIDTH * IMG_HEIGHT];
    if (read(img_fd, buffer, IMG_WIDTH * IMG_HEIGHT) != IMG_WIDTH * IMG_HEIGHT)
        errx(EXIT_FAILURE, "load_image() couldn't read the entire image");

    for (size_t i = 0; i < IMG_WIDTH * IMG_HEIGHT; ++i)
    {
        image->matrix[i][0] = (double)(buffer[i]) / MAX_BLACK;
    }
}

/**
  * Read the label file descriptor to read the byte corresponding to what the
  * image is representing (0 to 9)
  * Fill a matrix of size (10, 1) where the index of the value is set to 1
  */
void load_label(int lbl_fd, Matrix *expected)
{
    char label;
    if (read(lbl_fd, &label, sizeof(label)) != sizeof(label))
        errx(EXIT_FAILURE, "load_label() couldn't read the label");
    expected->matrix[(size_t)label][0] = 1;
}

/**
  * Allocate a freshly new TrainingData struct and fill it by reading the image
  * and label file descriptors
  */
TrainingData *load_image_label(int img_fd, int lbl_fd)
{
    TrainingData *data = malloc(sizeof(TrainingData));
    data->image = init_matrix2(IMG_WIDTH * IMG_HEIGHT, 1);
    data->expected = init_matrix2(NUMBER_DIGIT, 1);
    load_image(img_fd, data->image);
    load_label(lbl_fd, data->expected);
    return data;
}

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
size_t load_images(char *img_path, char *label_path, TrainingData ***data)
{
    if (data == NULL)
        errx(EXIT_FAILURE, "load_images(): invalid pointer");
    int img_fd = open(img_path, O_RDONLY);
    int lbl_fd = open(label_path, O_RDONLY);

    int nb_items = check_header(img_fd, lbl_fd);
    *data = malloc(nb_items * sizeof(TrainingData *));
    for (int i = 0; i < nb_items; ++i)
        (*data)[i] = load_image_label(img_fd, lbl_fd);

    close(img_fd);
    close(lbl_fd);
    return nb_items;
}

/**
  * Free all the dataset of size n
  */
void free_all_training_data(TrainingData **data, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        free_training_data(data[i]);
    free(data);
}