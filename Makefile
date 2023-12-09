CC = gcc -IIncludes -o Bin/gui
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image gtk+-3.0` -fsanitize=address
LDFLAGS = -fsanitize=address
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image gtk+-3.0`

SOURCE_DIR := ./Src

SRC = $(shell find $(SOURCE_DIR) -name "*.c" ! -name "img_action.c" ! -name "test.c")
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all:  gui

gui: ${OBJ}
	gcc  -IIncludes $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	rm Src/gui/*.o
	rm Src/Img_Processing/*.o
	rm Src/Img_Transformation/*.o
	rm Src/neural/*.o
	rm Src/Grid_Detection/*.o
	rm Src/Solver/*.o
