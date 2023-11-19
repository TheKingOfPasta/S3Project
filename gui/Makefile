CC = gcc
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image gtk+-3.0`
LDFLAGS =
LDLIBS = -lm -ldl `pkg-config --libs sdl2 SDL2_image gtk+-3.0`

SRC = gui.c ../img_processing/img_color.c ../img_processing/adaptive_thresholding.c ../img_processing/gaussian_blur.c ../img_processing/rotate.c ../img_processing/canny_edge_detector.c ../img_processing/invert_colors.c ../img_processing/img_color.c ../hough/utils.c ../hough/visualization.c ../hough/square_detection.c ../hough/hough.c ../hough/grid_detection.c ../img_processing/erosion.c
OBJ = ${SRC:.c=.o}

all: gui

gui: gui.o ../img_processing/img_color.o ../img_processing/adaptive_thresholding.o ../img_processing/gaussian_blur.o ../img_processing/rotate.o ../img_processing/canny_edge_detector.o ../img_processing/invert_colors.o ../img_processing/img_color.o ../hough/utils.o ../hough/visualization.o ../hough/square_detection.o ../hough/hough.o ../hough/grid_detection.o ../img_processing/erosion.o

.PHONY: clean

clean:
	rm *.o
	rm gui
