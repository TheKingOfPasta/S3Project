# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 -lm -ldl `pkg-config --cflags`
LDFLAGS =
LDLIBS = -lm -ldl `pkg-config --libs`

EXE = gauss

all: $(EXE)

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} $(EXE)

# END
