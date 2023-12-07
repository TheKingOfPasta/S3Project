# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 -lm -ldl
LDFLAGS =
LDLIBS = -lm -ldl

EXE = gauss

all: $(EXE)

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} $(EXE)

# END
