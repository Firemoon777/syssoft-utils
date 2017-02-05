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
	int fd_in, fd_out;
	ssize_t buff_readed, buff_w;
	if(argc != 3) {
		write(STDOUT_FILENO, "Usage: cp src dest\n", 19);  
		return 1;
	}
	
	fd_in = open(argv[1], O_RDONLY);
	if(fd_in < 0) {
		perror(argv[1]);
		return errno;
	}
	
	struct stat fileStat;
    if(stat(argv[1], &fileStat) < 0) { 
		return 3;
	}    
	fileStat.st_mode = fileStat.st_mode;
	
	fd_out = open(argv[2], O_CREAT | O_WRONLY | O_EXCL, fileStat.st_mode);
	if(fd_out < 0) {
		perror(argv[2]);
		return errno;
	}
	while((buff_readed = read(fd_in, buff, BUFFER_SIZE)) > 0) {
		if(buff_readed < 0) {
			perror("read");
			return errno;
		}
		buff_w = write(fd_out, buff, buff_readed);
		if(buff_w < 0) {
			perror("write");
			return errno;
		}
	}
	return 0;
}
