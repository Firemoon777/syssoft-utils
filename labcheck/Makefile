CC=gcc
CFLAGS=-g -O3 -Wall -Werror -pedantic -std=c89
LIBS=
SRC=src/labcheck.c src/lab4.c
LIBRESTRSRC=src/librestriction/librestriction.c
	
all: librestriction.so labcheck
	
labcheck: dir 
	$(CC) -o bin/$@ $(SRC) $(CFLAGS) $(LIBS)
	
librestriction.so: dir
	$(CC) $(CFLAGS) -shared -fPIC $(LIBRESTRSRC) -o bin/$@
	
dir: 
	mkdir -p bin
	
clean:
	rm bin/*

