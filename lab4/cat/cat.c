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

static void do_work(const char* filename) {
	char buff[BUFFER_SIZE+1];
	int fd;
	ssize_t buff_readed;
	if(strcmp(filename, "-") == 0) {
		fd = STDIN_FILENO;
	} else {
		fd = open(filename, O_RDONLY);
	}
	if(fd < 0) {
		perror(filename);	
		return;
	}
	memset(buff, 0, BUFFER_SIZE+1);
	while((buff_readed = read(fd, buff, BUFFER_SIZE)) > 0) {
		(void)write(STDOUT_FILENO, buff, (size_t)buff_readed);
		memset(buff, 0, BUFFER_SIZE);
	}
	(void)close(fd);
}

int main(int argc, char** argv) {
	int i = 1;
	if(argc == 1) {
		do_work("-");
	}
	for(i = 1; i < argc; i++) {
		do_work(argv[i]);
	}
	return 0;
}
