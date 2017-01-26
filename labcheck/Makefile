CC=gcc
CFLAGS=-g -O3 -Wall -Werror -pedantic -c -std=c89
LIBS=-lfuse  `pkg-config fuse --libs`
SRC=$(wildcard src/*.c) 
OBJ=$(SRC:src/%.c=build/%.o)
LIBRESTRSRC=$(wildcard src/librestriction/*.c)
	
all: librestriction.so
	
librestriction.so: dir
	$(CC) $(CFLAGS) -shared -fPIC $(LIBRESTRSRC) -o bin/$@
	
build/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ $<
	
dir: 
	mkdir -p build bin
	
clean:
	rm build/* bin/*

