CC = gcc
LD = gcc
OPT=-O3
# SSE_ARCH_FLAGS=-msse3

# ARCH=-m64 $(SSE_ARCH_FLAGS) -ftree-vectorize -fopt-info-vec-missed
CFLAGS=-std=gnu99 -g -Wall $(OPT) -fopenmp #-lblas
LDFLAGS = -fopenmp #-lblas
RM = /bin/rm -f
OBJS = main.o
EXECUTABLE = conj

conj:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(EXECUTABLE) -lm #-lblas


main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
