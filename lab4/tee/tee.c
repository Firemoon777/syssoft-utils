#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>

#define BUFFER_SIZE 80

int main(int argc, char** argv) {
	char buff[BUFFER_SIZE];
	int* fd;
	ssize_t buff_readed;
	size_t i = 1;
	
	fd = (int*)malloc(argc * sizeof(int));
	
	for(i = 1; i < argc; i++) {
		fd[i] = open(argv[i], O_WRONLY | O_CREAT | O_EXCL, 0644);
		if(fd[i] < 0) {
			perror(argv[i]);	
			continue;
		}
	}
	fd[0] = STDOUT_FILENO;
	
	while((buff_readed = read(STDIN_FILENO, buff, BUFFER_SIZE)) > 0) {
		for(i = 0; i < argc; i++) {
			if(fd[i] > 0) {
				write(fd[i], buff, buff_readed);
			}
		}
	}
	
	free(fd);
	
	return 0;
}
