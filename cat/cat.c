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
	char buff[BUFFER_SIZE+1];
	int fd;
	ssize_t buff_readed;
	size_t i;
	if(argc == 1) {
		argc++;
		*(argv + 1) = "-\0";
	}
	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-") == 0) {
			fd = STDIN_FILENO;
		} else {
			fd = open(argv[i], O_RDONLY);
		}
		if(fd < 0) {
			perror(argv[i]);	
		}
		memset(buff, 0, BUFFER_SIZE+1);
		while((buff_readed = read(fd, buff, BUFFER_SIZE)) > 0) {
			write(STDOUT_FILENO, buff, buff_readed);
			memset(buff, 0, BUFFER_SIZE);
		}
	}
	return 0;
}
