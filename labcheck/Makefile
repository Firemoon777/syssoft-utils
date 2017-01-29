CC=gcc
CFLAGS=-g -O3 -Wall -Werror -pedantic -std=c89
LIBS=
SRC=src/labcheck.c src/lab4.c
OBJ=$(SRC:src/%.c=build/%.o)
LIBRESTRSRC=src/librestriction/librestriction.c
	
all: librestriction.so labcheck
	
labcheck: $(OBJ)
	$(CC) -o bin/$@ $(OBJ) $(LIBS)
	
build/%.o: src/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
librestriction.so: dir
	$(CC) $(CFLAGS) -shared -fPIC $(LIBRESTRSRC) -o bin/$@
	
dir: 
	mkdir -p bin build
	
clean:
	rm bin/* build/*

