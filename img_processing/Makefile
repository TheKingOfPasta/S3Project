CC = gcc
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`-fsanitizer=address
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SRC = img_color.c img_action.c adaptive_thresholding.c gaussian_blur.c rotate.c canny_edge_detector.c invert_colors.c img_color.c ../hough/utils.c ../hough/visualization.c ../hough/square_detection.c ../hough/hough.c ../hough/grid_detection.c wrapping.c erosion.c
OBJ = ${SRC:.c=.o}

all: img_action

img_action: ${OBJ}

.PHONY: clean

clean:
	rm *.o
	rm img_action
	rm ../hough/*.o
