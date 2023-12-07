CC = gcc
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`-fsanitize=address
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SRC = inverse.c
OBJ = ${SRC:.c=.o}

all: inverse

inverse: ${OBJ}

.PHONY: clean

clean:
	rm *.o
	rm inverse