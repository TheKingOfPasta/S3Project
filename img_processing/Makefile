CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

SRC = img_utils.c img_color.c img_action.c
OBJ = ${SRC:.c=.o}

all: img_color img_action

img_color: img_utils.o img_color.o

img_action:img_utils.o  img_action.o

.PHONY: clean

clean: ${RM} ${OBJ}
