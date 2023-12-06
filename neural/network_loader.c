# include "network_loader.h"

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
    write(fd, &a->w, sizeof(a->w));
    write(fd, &a->h, sizeof(a->h));

    for (size_t i = 0; i < a->w; ++i)
    {
        for (size_t j = 0; j < a->h; ++j)
        {
            size_t s = sizeof(a->values[i][j]);
            if (write(fd, &(a->values[i][j]), s) < (ssize_t)s)
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
    Matrix *a = new_Matrix(m, n);

    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            if (read(fd, &a->values[i][j], sizeof(double)) <
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
void write_weights(int fd, Network *network)
{
  for (Node_m* curr = network->weights->head; curr; curr = curr->next)
    write_matrix(fd, curr->m);
}

/**
  * Read the weight matrices from the given file one by one into the network.
  * num_layers - 1 matrices are expected.
  *
  * Fail if a matrix could not be read
  */
void read_weights(int fd, Network *network)
{
    for (Node_m* curr = network->weights->head; curr; curr = curr->next)
      curr->m = read_matrix(fd);
}

/**
  * Write the biases matrices of the given network one by one into the file.
  * num_layers - 1 matrices are written.
  *
  * Fail if a matrix could not be written
  */
void write_biases(int fd, Network *network)
{
  for (Node_m* curr = network->biases->head; curr; curr = curr->next)
    write_matrix(fd, curr->m);
}

/**
  * Read the biases matrices from the given file one by one into the network.
  * num_layers - 1 matrices are expected.
  *
  * Fail if a matrix could not be read
  */
void read_biases(int fd, Network *network)
{
    for (Node_m* curr = network->biases->head; curr; curr = curr->next)
        curr->m = read_matrix(fd);
}

/**
  * Write the number of layers of the network into the file,
  * followed by the number of neurons in each layer
  */
void write_layers(int fd, Network *network)
{
    write(fd, &network->num_layers, sizeof(network->num_layers));
    for (DoubleNode* curr = network->sizes->head; curr; curr = curr->next)
        write(fd, &(curr->d), sizeof(size_t));
}

/**
  * Reverse operation of write_layers.
  * Read the number of layers then the number of neurons in each.
  *
  * Allocate enough memory for:
  * - sizes
  * - weights
  * - biases
  */
void read_layers(int fd, Network *network)
{
  size_t num_layers;
  if (read(fd, &num_layers, sizeof(size_t)) < 8)
    err(EXIT_FAILURE, "Could not read num_layers");
  network->num_layers = num_layers;

  network->sizes = malloc(num_layers * sizeof(size_t));
  network->weights = malloc((num_layers - 1) * sizeof(double *));
  network->biases = malloc((num_layers - 1) * sizeof(double *));
  assert(network->sizes != NULL);

  for (DoubleNode* curr = network->sizes->head; curr; curr = curr->next)
  {
    size_t r = read(fd, &(curr->d), sizeof(size_t));
    if (r < 8)
        err(EXIT_FAILURE, "Could not read layers sizes, read %zu", r);
  }
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
  * - weights
  * - biases
  *
  * Fail if a field could not be written correctly or the given path is invalid
  */
void save_network(Network *network, char *path)
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
Network *load_network(char *path)
{
  int fd = open(path, O_RDONLY);
  if (fd < 0)
    err(EXIT_FAILURE, "save_network() - open()");

  Network *n = malloc(sizeof(Network));

  printf("Getting headers\n");
  read_header(fd);
  printf("Got headers\nGetting layers\n");
  read_layers(fd, n);
  printf("Got layers\nGetting weights\n");
  read_weights(fd, n);
  printf("Got weights\nGetting biases\n");
  read_biases(fd, n);
  printf("Got biases\n");

  //NN_print_biases(n);
  close(fd);
  return n;
}
