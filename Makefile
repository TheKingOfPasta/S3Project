CC = gcc -IIncludes
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image gtk+-3.0` -fsanitize=address
LDFLAGS = -fsanitize=address
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image gtk+-3.0`

SOURCE_DIR := Src
SRC = $(shell find Src/Img_Processing -name "*.c" ! -name "Src/Img_Processing/img_action.c") \
	  $(shell find Src/Img_Transformation -name "*.c" ) \
	  $(shell find Src/Grid_Detection -name "*.c" )	\
OBJ = ${SRC:.c=.o}
EXE = img_action gui networkTest

all:  ${EXE}

img_action: ${OBJ} Src/Img_Processing/img_action.o

gui: $(shell find Src/Solver -name "*.o") Src/gui/gui.o \
	Src/neural/second_network.o Src/neural/network_loader.o Src/Utils/matrix.o

networkTest: Src/neural/second_network.o Src/neural/test.o Src/neural/network_loader.o Src/Utils/matrix.o
.PHONY: clean

clean:
	rm ****.o
	rm Src/gui/gui
	rm Src/Img_Processing/img_action
	rm Src/neural/networkTest
