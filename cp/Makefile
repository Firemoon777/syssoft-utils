CC=gcc
CFLAGS=-Wall -Werror -c
PROJECT=cp

all: cp.o
	$(CC) cp.o -o $(PROJECT)
	
cat.o: cp.c
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -f *.o $(PROJECT)


