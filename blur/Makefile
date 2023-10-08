CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

SRC = gaussian_blur.c
OBJ = ${SRC:.c=.o}

all: gaussian_blur

gaussian_blur: gaussian_blur.o

.PHONY: clean

clean:
	rm *.o
	rm gaussian_blur