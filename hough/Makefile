# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 -lm -ldl -g `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = -lm -ldl -g `pkg-config --libs sdl2 SDL2_image`

SRC = grid_detection.c hough.c square_detection.c utils.c visualization.c
OBJ = ${SRC:.c=.o}

all: grid_detection

grid_detection: $(OBJ)

.PHONY: clean

clean:
	rm *.o
	rm grid_detection

# END
