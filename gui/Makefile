CC = gcc
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SRC = gui.c draw_interface.c
OBJ = ${SRC:.c=.o}

all: gui

gui: gui.o draw_interface.o

.PHONY: clean

clean:
	rm *.o
	rm gui
