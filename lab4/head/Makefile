CC=gcc
CFLAGS=-Wall -Werror -c
PROJECT=head

all: head.o
	$(CC) head.o -o $(PROJECT)
	
head.o: head.c
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -f *.o $(PROJECT)


