# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 -lm -ldl -g `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = -lm -ldl -g `pkg-config --libs sdl2 SDL2_image`

EXE = hough

all: $(EXE)

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} $(EXE)

# END
