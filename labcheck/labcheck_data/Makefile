CC=gcc
CFLAGS=-g -O3 -Wall -Werror -pedantic -std=c89
	
all: lab4_preload_x86.so lab4_preload_x64.so

lab4_preload_x86.so: lab4_preload.c
	$(CC) $(CFLAGS) -shared -fPIC $^ -o $@
	
lab4_preload_x64.so: lab4_preload.c
	$(CC) $(CFLAGS) -shared -fPIC -m64 $^ -o $@
	
clean:
	rm lab4_preload_x86.so lab4_preload_x64.so 

