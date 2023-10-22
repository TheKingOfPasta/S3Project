# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 -lm -ldl `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = -lm -ldl `pkg-config --libs sdl2 SDL2_image`

EXE = hough GPT

all: $(EXE)

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} $(EXE)

# END
