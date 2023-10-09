CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 -g
LDFLAGS = -lm
LDLIBS =

SRC = network.c testsuite.c layer.c neuron.c helper.c
OBJ = ${SRC:.c=.o}

all: testsuite

testsuite: testsuite.o network.o layer.o neuron.o helper.o

network: network.o


.PHONY: clean

clean: 
	${RM} ${OBJ}
