CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 
LDFLAGS = -lm
LDLIBS =

SRC = network.c testsuite.c
OBJ = ${SRC:.c=.o}

all: testsuite

testsuite: testsuite.o


.PHONY: clean

clean: rm ${OBJ}
