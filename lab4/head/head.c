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

static void do_task(const char* filename, int nl) {
	char buff[BUFFER_SIZE+1];
	int fd;
	const size_t bound = 10;
	ssize_t buff_readed;
	size_t k, j;
	if(strcmp(filename, "-") == 0) {
		fd = STDIN_FILENO;
	} else {
		fd = open(filename, O_RDONLY);
	}
	if(fd < 0) {
		perror(filename);	
		return;
	}
	if(nl != 0) {
		(void)write(STDOUT_FILENO, "==> ", 4);
		(void)write(STDOUT_FILENO, filename, strlen(filename));
		(void)write(STDOUT_FILENO, " <==\n", 5);
	}
	k = 0;
	while((buff_readed = read(fd, buff, BUFFER_SIZE)) > 0 && k < bound) {
		for(j = 0; j < (size_t)buff_readed; j++) {
			if(buff[j] == '\n') {
				k++;
				if(k == bound) {
					buff_readed = (ssize_t)j + 1;
					break;
				}
			}
		}
		(void)write(STDOUT_FILENO, buff, (size_t)buff_readed);
	}
	if(nl != 0) {
		(void)write(STDOUT_FILENO, "\n", 1);
	}
	(void)close(fd);
}

int main(int argc, char** argv) {
	size_t i = 1;
	if(argc == 1) {
		do_task("-", 0);
	}
	for(i = 1; i < (size_t)argc; i++) {
		do_task(argv[i], argc > 2 && (size_t)argc != i + 1 ? 1 : 0);
	}
	return 0;
}
