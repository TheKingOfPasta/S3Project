CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 -g -fsanitize=address
LDFLAGS = -lm -fsanitize=address
LDLIBS =

SRC = test.c network.c test.c helper.c mnist_reader.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: test


test: test.o network.o helper.o mnist_reader.o


.PHONY: clean

clean: 
	${RM} ${OBJ} ${EXE}
