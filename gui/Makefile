CC = gcc
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -g -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image SDL2_ttf gtk+-3.0` -fsanitize=address
LDFLAGS =
LDLIBS = -lm -ldl `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk+-3.0` -fsanitize=address

SRC = gui.c ../img_processing/img_color.c  ../img_processing/adaptive_thresholding.c ../img_processing/gaussian_blur.c  ../img_processing/rotate.c ../img_processing/canny_edge_detector.c  ../img_processing/invert_colors.c ../img_processing/img_color.c ../hough/utils.c  ../hough/visualization.c ../hough/square_detection.c ../hough/hough.c  ../hough/grid_detection.c ../img_processing/erosion.c ../img_processing/wrapping.c ../splitting/split.c ../solver/solver.c  ../solver/parser.c ../neural/second_network.c ../neural/network_loader.c ../resize/resize.c ../utils/matrix.c
OBJ = ${SRC:.c=.o}

all: gui

gui: ${OBJ}

.PHONY: clean

clean:
	rm *.o
	rm gui
