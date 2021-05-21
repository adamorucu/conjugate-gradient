CC = gcc
LD = gcc
CFLAGS = -O0
LDFLAGS =
RM = /bin/rm -f
OBJS = main.o
EXECUTABLE = conj

conj:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(EXECUTABLE) -lm


main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
