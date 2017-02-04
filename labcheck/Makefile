CC=gcc
CFLAGS=-g -O3 -Wall -Werror -pedantic -std=c89
	
all: lab4_preload.so 
	
lab4_preload.so: 
	$(CC) $(CFLAGS) -shared -fPIC lab4_preload.c -o $@
	
clean:
	rm lab4_preload.so 

