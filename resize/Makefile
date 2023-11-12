CC = gcc
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`-fsanitizer=address
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SRC = resize.c
OBJ = ${SRC:.c=.o}

all: resize

img_action: ${OBJ}

.PHONY: clean

clean:
	rm *.o
	rm resize
