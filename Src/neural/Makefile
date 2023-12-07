CC = gcc -I../../Includes
CPPFLAGS =
CFLAGS = -Wall -Wextra -O0 -g `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS = -lm -fsanitize=address -lpthread `pkg-config --libs sdl2 SDL2_image SDL2_ttf`
LDLIBS =

SRC = second_network.c test.c network_loader.c ../Utils/matrix.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: test

test: ${OBJ}


.PHONY: clean

clean:
	${RM} ${OBJ} ${EXE}
