CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O0 -g
LDFLAGS = -lm -fsanitize=address -lpthread
LDLIBS =

SRC = second_network.c test.c helper.c network_loader.c mnist_reader.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: test

test: ${OBJ}


.PHONY: clean

clean:
	${RM} ${OBJ} ${EXE}
