CC=gcc
CFLAGS=-Wall -Werror -c
PROJECT=cat

all: cat.o
	$(CC) cat.o -o $(PROJECT)
	
cat.o: cat.c
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -f *.o $(PROJECT)


