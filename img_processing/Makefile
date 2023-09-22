CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

SRC = img_color.c
OBJ = ${SRC:.c=.o}

img_color: img_color.o

.PHONY: clean

clean: ${RM} ${OBJ}
