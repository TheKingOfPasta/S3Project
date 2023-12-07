# pragma once
# include <fcntl.h>
# include <err.h>
#include <assert.h>

# include "second_network.h"
# include <unistd.h>
# include <stdio.h>


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
void save_network(Network *network, char *path);

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
Network *load_network(char *path);


// ============================================================================
// ==================== INTERNAL FUNCTIONS ====================================
// ============================================================================
// Functions that may depends on another ones to function correctly...
// The above ones are probably what you're looking for.

/**
  * Write the magic number to the file (8629)
  */
void write_header(int fd);

/**
  * Read the magic number from the file descriptor.
  * Throw an error if it is not the magic number
  */
void read_header(int fd);

/**
  * Write the given matrix into the file.
  *
  * The format is the following:
  * The numbers of rows (m) then columns (n) is written
  * then followed by the matrix values row-wise.
  *
  * Fail if the matrix could not be entirely written.
  */
void write_matrix(int fd, Matrix *a);

/**
  * Reverse operation of write_matrix.
  * Read the file at the current position and expect a matrix of it.
  * Return a freshly allocated matrix pointer contaning the matrix at this position.
  *
  * Fail if the matrix dimensions or values could not be read.
  */
Matrix *read_matrix(int fd);

/**
  * Write the weight matrices of the given network one by one into the file.
  * num_layers - 1 matrices are written.
  *
  * Fail if a matrix could not be written
  */
void write_weights(int fd, Network *network);

/**
  * Read the weight matrices from the given file one by one into the network.
  * num_layers - 1 matrices are expected.
  *
  * Fail if a matrix could not be read
  */
void read_weights(int fd, Network *network);

/**
  * Write the biases matrices of the given network one by one into the file.
  * num_layers - 1 matrices are written.
  *
  * Fail if a matrix could not be written
  */
void write_biases(int fd, Network *network);

/**
  * Read the biases matrices from the given file one by one into the network.
  * num_layers - 1 matrices are expected.
  *
  * Fail if a matrix could not be read
  */
void read_biases(int fd, Network *network);

/**
  * Write the number of layers of the network into the file,
  * followed by the number of neurons in each layer
  */
void write_layers(int fd, Network *network);

/**
  * Reverse operation of write_layers.
  * Read the number of layers then the number of neurons in each.
  *
  * Allocate enough memory for:
  * - sizes
  * - weights2
  * - biases2
  */
void read_layers(int fd, Network *network);
