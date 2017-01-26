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
	ssize_t buff_readed;
	//size_t i;
	if(argc != 3) {
		err_print(argv[0], "Insufficient arguments\n", "Must be: src dest\n");
		return 1;
	}
	fd_in = open(argv[1], O_RDONLY);
	if(fd_in < 0) {
		err_parse(errno, argv[0], argv[1]);
		return errno;
	}
	struct stat fileStat;
    if(stat(argv[1], &fileStat) < 0) { 
		return 3;
	}    
	fileStat.st_mode = fileStat.st_mode & (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
	fd_out = open(argv[2], O_RDONLY);
	if(fd_out > 0) {
		err_parse(EEXIST, argv[0], argv[2]);
		return 2;
	}
	close(fd_out);
	fd_out = open(argv[2], O_CREAT | O_WRONLY, fileStat.st_mode);
	if(fd_out < 0) {
		err_parse(errno, argv[0], argv[2]);
		return errno;
	}
	while((buff_readed = read(fd_in, buff, BUFFER_SIZE)) > 0) {
		write(fd_out, buff, buff_readed);
	}
	return 0;
}
