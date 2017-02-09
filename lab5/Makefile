CC=gcc
CFLAGS=-Wall -Werror -c -Wextra -std=c89

all: task9_server

task1_server: task1_server.o
	$(CC) $^ -o $@
		
task1_server.o: task1_server.c
	$(CC) $(CFLAGS) $^ -o $@

task1_client: task1_client.o
	$(CC) $^ -o $@
		
task1_client.o: task1_client.c
	$(CC) $(CFLAGS) $^ -o $@

task9_server: task9_server.o
	$(CC) $^ -o $@
		
task9_server.o: task9_server.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm *.o
