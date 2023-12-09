CC = gcc 
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image gtk+-3.0` 
LDFLAGS = LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image gtk+-3.0`

SOURCE_DIR := ./Src
BIN_DIR := ./bin

SRC = $(shell find $(SOURCE_DIR) -name "*.c" ! -name "img_action.c" ! -name "test.c")
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all:  gui

gui: 
	cd Src/gui/ && make
	cd Src/gui && ./gui

.PHONY: clean

clean:
	rm Src/gui/*.o
	rm Src/Img_Processing/*.o
	rm Src/Img_Transformation/*.o
	rm Src/neural/*.o
	rm Src/Grid_Detection/*.o
	rm Src/Solver/*.o
