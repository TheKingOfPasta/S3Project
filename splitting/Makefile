CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

SRC = split.c
OBJ = ${SRC:.c=.o}

all: split

split: split.o

.PHONY: clean

clean:
	rm *.o
	rm split
