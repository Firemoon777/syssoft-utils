CC=gcc
CFLAGS=-g -O3 -Wall -Werror -pedantic -std=c89
	
all: lab4_preload.so 
	
lab4_preload.so: lab4_preload.c
	$(CC) $(CFLAGS) -shared -fPIC $^ -o $@
	
clean:
	rm lab4_preload.so 

