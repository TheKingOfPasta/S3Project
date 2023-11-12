CC = gcc
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`-fsanitizer=address
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SRC = split.c
OBJ = ${SRC:.c=.o}

all: split

img_action: ${OBJ}

.PHONY: clean

clean:
	rm *.o
	rm split
