# pragma once
# include <fcntl.h>
# include <err.h>
#include <assert.h>

# include "second_network.h"
# include <unistd.h>
# include <stdio.h>

void Save_Network(Network* n, char* path);

Network* Load_Network(char* path);