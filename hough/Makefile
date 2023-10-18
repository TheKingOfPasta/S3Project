CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 -lm -ldl `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS =-lm -ldl `pkg-config --libs sdl2 SDL2_image`

SRC = hough.c
OBJ = ${SRC:.c=.o}

all: hough

split: hough.o

.PHONY: clean

clean:
	rm *.o
	rm hough
