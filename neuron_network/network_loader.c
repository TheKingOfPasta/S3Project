# include "network_loader.h"
# include "network.h"
# include <unistd.h>
# include <stdio.h>

# define MAGIC 8629

/**
  * Write the magic number to the file (8629)
  */
void write_header(int fd)
{
    int magic = MAGIC;
    ssize_t written = write(fd, &magic, sizeof(magic));
    if (written < (ssize_t)sizeof(MAGIC))
        err(EXIT_FAILURE, "Could not write the network's header");
}

/**
  * Read the magic number from the file descriptor.
  * Throw an error if it is not the magic number
  */
void read_header(int fd)
{
    int magic;
    ssize_t readb = read(fd, &magic, sizeof(magic));
    if (readb < (ssize_t)sizeof(MAGIC))
        err(EXIT_FAILURE, "Could not read the network's header");
    if (magic != MAGIC)
        err(EXIT_FAILURE, "Wrong magic number");
    printf("Magic: %i\n", magic);
}

/**
  * Write the given matrix into the file.
  *
  * The format is the following:
  * The numbers of rows (m) then columns (n) is written
  * then followed by the matrix values row-wise.
  *
  * Fail if the matrix could not be entirely written.
  */
void write_matrix(int fd, Matrix *a)
{
    write(fd, &a->m, sizeof(a->m));
    write(fd, &a->n, sizeof(a->n));

    for (size_t i = 0; i < a->m; ++i)
    {
        for (size_t j = 0; j < a->n; ++j)
        {
            size_t s = sizeof(a->matrix[i][j]);
            if (write(fd, &(a->matrix[i][j]), s) < (ssize_t)s)
                err(EXIT_FAILURE, "write_matrix");
        }
    }
}

/**
  * Reverse operation of write_matrix.
  * Read the file at the current position and expect a matrix of it.
  * Return a freshly allocated matrix pointer contaning the matrix at this position.
  *
  * Fail if the matrix dimensions or values could not be read.
  */
Matrix *read_matrix(int fd)
{
    size_t m, n;
    if (read(fd, &m, sizeof(size_t)) < (ssize_t)(sizeof(size_t)))
        err(EXIT_FAILURE, "Could not read dimensions of matrix");
    if (read(fd, &n, sizeof(size_t)) < (ssize_t)(sizeof(size_t)))
        err(EXIT_FAILURE, "Could not read dimensions of matrix");
    Matrix *a = init_matrix2(m, n);

    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            if (read(fd, &a->matrix[i][j], sizeof(double)) <
                    (ssize_t)(sizeof(double)))
                err(EXIT_FAILURE, "Could not read matrix");
        }
    }
    return a;
}

/**
  * Write the weight matrices of the given network one by one into the file.
  * num_layers - 1 matrices are written.
  *
  * Fail if a matrix could not be written
  */
void write_weights(int fd, NN_Network *network)
{
    for (size_t i = 0; i < network->num_layers - 1; ++i)
        write_matrix(fd, network->weights2[i]);
}

/**
  * Read the weight matrices from the given file one by one into the network.
  * num_layers - 1 matrices are expected.
  *
  * Fail if a matrix could not be read
  */
void read_weights(int fd, NN_Network *network)
{
    for (size_t i = 0; i < network->num_layers - 1; ++i)
        network->weights2[i] = read_matrix(fd);
}

/**
  * Write the biases matrices of the given network one by one into the file.
  * num_layers - 1 matrices are written.
  *
  * Fail if a matrix could not be written
  */
void write_biases(int fd, NN_Network *network)
{
    for (size_t i = 0; i < network->num_layers - 1; ++i)
        write_matrix(fd, network->biases2[i]);
}

/**
  * Read the biases matrices from the given file one by one into the network.
  * num_layers - 1 matrices are expected.
  *
  * Fail if a matrix could not be read
  */
void read_biases(int fd, NN_Network *network)
{
    for (size_t i = 0; i < network->num_layers - 1; ++i)
        network->biases2[i] = read_matrix(fd);
}

/**
  * Write the number of layers of the network into the file,
  * followed by the number of neurons in each layer
  */
void write_layers(int fd, NN_Network *network)
{
    write(fd, &network->num_layers, sizeof(network->num_layers));
    for (size_t i = 0; i < network->num_layers; ++i)
        write(fd, &network->sizes[i], sizeof(size_t));
}

/**
  * Reverse operation of write_layers.
  * Read the number of layers then the number of neurons in each.
  *
  * Allocate enough memory for:
  * - sizes
  * - weights2
  * - biases2
  */
void read_layers(int fd, NN_Network *network)
{
    size_t num_layers;
    if (read(fd, &num_layers, sizeof(size_t)) < 8)
        err(EXIT_FAILURE, "Could not read num_layers");
    network->num_layers = num_layers;

    network->sizes = malloc(num_layers * sizeof(size_t));
    network->weights2 = malloc((num_layers - 1) * sizeof(double *));
    network->biases2 = malloc((num_layers - 1) * sizeof(double *));
    assert(network->sizes != NULL);
    for (size_t i = 0; i < num_layers; ++i)
        if (read(fd, &network->sizes[i], sizeof(size_t)) < 8)
            err(EXIT_FAILURE, "Could not read layers sizes");
}

/**
  * Write the given network into a file located in path
  *
  * The file written is in a binary format. A magic number identifies it and
  * should not be modified manually as it may cause unexpected issues.
  *
  * Every fields of the network is written inside:
  * - num_layers
  * - sizes
  * - weights2
  * - biases2
  *
  * Fail if a field could not be written correctly or the given path is invalid
  */
void save_network(NN_Network *network, char *path)
{
    int fd = creat(path, S_IRUSR | S_IWUSR);
    if (fd < 0)
        err(EXIT_FAILURE, "save_network() - creat()");
    write_header(fd);
    write_layers(fd, network);
    write_weights(fd, network);
    write_biases(fd, network);

    close(fd);
}

/**
  * Read the file at the path given and return the network contained inside
  *
  * The path should be a file written in the save_network(2) format.
  * Return a freshly allocated Network.
  *
  * Fail if:
  * - File could not be read
  * - File is not recognisable (Magic number unknown)
  * - Unable to allocate enough memory
  * - File is corrupted/has been modified
  */
NN_Network *load_network(char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
        err(EXIT_FAILURE, "save_network() - open()");

    NN_Network *n = malloc(sizeof(NN_Network));

    read_header(fd);
    read_layers(fd, n);
    read_weights(fd, n);
    read_biases(fd, n);

    //NN_print_biases(n);
    close(fd);
    return n;

}
