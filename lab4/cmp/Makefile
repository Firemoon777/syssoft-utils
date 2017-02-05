CC=gcc
CFLAGS=-Wall -Werror -c
PROJECT=cmp

all: $(PROJECT).o
	$(CC) $^ -o $(PROJECT)
	
$(PROJECT).o: $(PROJECT).c
	$(CC) $(CFLAGS) $^ -o $@
	
clean:
	rm -f *.o $(PROJECT)


