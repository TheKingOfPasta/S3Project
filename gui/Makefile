CC = gcc
CPPFLAGS =
CFLAGS = -lm -ldl -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image SDL2_ttf gtk4`
LDFLAGS =
LDLIBS = -lm -ldl `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk4`

SRC = gui.c draw_text.c ../img_processing/adaptive_thresholding.c ../img_processing/canny_edge_detector.c ../img_processing/gaussian_blur.c ../img_processing/rotate.c
OBJ = ${SRC:.c=.o}

all: gui

gui: gui.o draw_text.o ../img_processing/adaptive_thresholding.o ../img_processing/canny_edge_detector.o ../img_processing/gaussian_blur.o ../img_processing/rotate.o

.PHONY: clean

clean:
	rm *.o
	rm ../img_processing/*.o
	rm gui
