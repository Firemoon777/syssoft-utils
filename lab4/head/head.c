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
	const int bound = 10;
	ssize_t buff_readed;
	size_t i = 1, k, j;
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
			continue;
		}
		if(argc > 2) {
			write(STDOUT_FILENO, "==> ", 4);
			write(STDOUT_FILENO, argv[i], strlen(argv[i]));
			write(STDOUT_FILENO, " <==\n", 5);
		}
		k = 0;
		while((buff_readed = read(fd, buff, BUFFER_SIZE)) > 0 && k < bound) {
			for(j = 0; j < buff_readed; j++) {
				if(buff[j] == '\n') {
					k++;
					if(k == bound) {
						buff_readed = j+1;
						break;
					}
				}
			}
			write(STDOUT_FILENO, buff, buff_readed);
		}
		if(argc > 2 && argc != i + 1) {
			write(STDOUT_FILENO, "\n", 1);
		}
	}
	return 0;
}
