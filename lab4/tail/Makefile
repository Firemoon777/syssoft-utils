CC=gcc
CFLAGS=-Wall -Werror -c
PROJECT=tail

all: $(PROJECT).o
	$(CC) $< -o $(PROJECT)
	
$(PROJECT).o: $(PROJECT).c
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -f *.o $(PROJECT)


