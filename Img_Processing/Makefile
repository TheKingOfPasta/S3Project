CC = gcc -I../Includes
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -fsanitize=address
LDFLAGS = -fsanitize=address
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SOURCE_DIR := ../.
SRC = $(shell find . -name "*.c" ) $(shell find ../Grid_Detection -name "*.c" ) $(shell find ../Img_Transformation -name "*.c" )
OBJ = ${SRC:.c=.o}

all: img_action

img_action: ${OBJ}

.PHONY: clean

clean:
	rm img_action
	rm *.o