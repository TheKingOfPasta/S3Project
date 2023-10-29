# pragma once
# include "network.h"
# include <unistd.h>
# include <fcntl.h>
# include <err.h>

void write_header(int fd);
void save_network(NN_Network *network, char *path);
NN_Network *load_network(char *path);

