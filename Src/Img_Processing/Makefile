CC = gcc -I../../Includes
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -fsanitize=address
LDFLAGS = -fsanitize=address
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SOURCE_DIR := ..
SRC = $(shell find ${SOURCE_DIR}/Img_Processing -name "*.c" ! -name "img_action.c") \
	  $(shell find ${SOURCE_DIR}/Img_Transformation -name "*.c" ) 	\
	  $(shell find ${SOURCE_DIR}/Grid_Detection -name "*.c" ) \
	  ../Utils/matrix.c ../Utils/inverse.c
OBJ = ${SRC:.c=.o}

all: img_action

img_action: ${OBJ}

.PHONY: clean

clean:
	rm img_action
	rm **.o