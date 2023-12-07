CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O0 -g -fsanitize=address
LDFLAGS = -lm -fsanitize=address -lpthread
LDLIBS =

SRC = network_loader.c test.c network.c test.c helper.c mnist_reader.c cross_entropy_cost.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: test


test: test.o network.o helper.o mnist_reader.o cross_entropy_cost.o network_loader.o


.PHONY: clean

clean: 
	${RM} ${OBJ} ${EXE}
