CC = gcc
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -g -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image SDL2_ttf gtk+-3.0` -fsanitize=address
LDFLAGS =
LDLIBS = -lm -ldl `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk+-3.0` -fsanitize=address

SRC = gui.c ../img_processing/img_color.c  ../img_processing/adaptive_thresholding.c ../img_processing/gaussian_blur.c  ../img_processing/rotate.c ../img_processing/canny_edge_detector.c  ../img_processing/invert_colors.c ../img_processing/img_color.c ../hough/utils.c  ../hough/visualization.c ../hough/square_detection.c ../hough/hough.c  ../hough/grid_detection.c ../img_processing/erosion.c ../img_processing/wrapping.c ../splitting/split.c ../solver/solver.c  ../solver/parser.c ../neuron_network/helper.c ../neuron_network/network.c  ../neuron_network/network_loader.c
OBJ = ${SRC:.c=.o}

all: gui

gui: gui.o ../img_processing/img_color.o ../img_processing/adaptive_thresholding.o ../img_processing/gaussian_blur.o ../img_processing/rotate.o ../img_processing/canny_edge_detector.o ../img_processing/invert_colors.o ../img_processing/img_color.o ../hough/utils.o ../hough/visualization.o ../hough/square_detection.o ../hough/hough.o ../hough/grid_detection.o ../img_processing/erosion.o ../img_processing/wrapping.o ../splitting/split.o ../solver/solver.o ../solver/parser.o ../neuron_network/helper.c ../neuron_network/network.c  ../neuron_network/network_loader.c

.PHONY: clean

clean:
	rm *.o
	rm gui
