CC = gcc
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SRC = img_utils.c img_color.c img_action.c adaptive_thresholding.c gaussian_blur.c rotate.c canny_edge_detector.c invert_colors.c
OBJ = ${SRC:.c=.o}

all: img_color img_action

img_color: img_utils.o img_color.o

img_action:img_utils.o  img_action.o adaptive_thresholding.o gaussian_blur.o rotate.o canny_edge_detector.o invert_colors.o

.PHONY: clean

clean:
	rm *.o
	rm img_action
	rm img_color
