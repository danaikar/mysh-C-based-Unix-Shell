CC = gcc
DEPS = headers.h
OBJ = shell.o parser.o redir.o pipes.o bg.o alias.o wild.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
		gcc -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o 

run:
	make all && ./all