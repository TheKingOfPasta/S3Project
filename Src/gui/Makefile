CC = gcc -I../../Includes
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -g -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image SDL2_ttf gtk+-3.0` -fsanitize=address
LDFLAGS =
LDLIBS = -lm -ldl `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk+-3.0` -fsanitize=address

SRC = $(shell find ../Img_Processing -name "*.c" ! -name "img_action.c" )	\
 	  $(shell find ../Img_Transformation -name "*.c" )		\
	  $(shell find ../Grid_Detection -name "*.c" )		\
	  $(shell find ../Solver -name "*.c")		\
	gui.c ../neural/second_network.c ../neural/network_loader.c ../Utils/matrix.c ../Utils/inverse.c

OBJ = ${SRC:.c=.o}

all: gui

gui: ${OBJ}

.PHONY: clean

clean:
	rm *.o
	rm gui
