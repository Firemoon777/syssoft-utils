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

int print_err(char* prog, char* file, char* msg) {
	size_t string_size = (strlen(prog) + strlen(file) + strlen(msg)) + 2;
	char* result = (char*)malloc(string_size * sizeof(char));
	if(result == NULL) {
		exit(ENOMEM);
	}
	strcpy(result, prog);
	strcat(result, ": ");
	strcat(result, file);
	strcat(result, msg);
	write(STDERR_FILENO, result, string_size);
	free(result);
	return 0;
}

int main(int argc, char** argv) {
	char buff[BUFFER_SIZE+1];
	int fd;
	ssize_t buff_readed;
	if(argc == 1) {
		argc++;
		*(argv + 1) = "-\0";
	}
	for(size_t i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-") == 0) {
			fd = STDIN_FILENO;
		} else {
			fd = open(argv[i], O_RDONLY);
		}
		if(fd < 0) {
			switch(errno) {
				case ENOENT:
					print_err(argv[0], argv[i], " no such file\n");
					continue;
					
				case EACCES:
					print_err(argv[0], argv[i], " permission denied\n");
					continue;
					
				default:
					print_err(argv[0], argv[i], " unknown error\n");
					continue;
			}
		}
		memset(buff, 0, BUFFER_SIZE+1);
		while((buff_readed = read(fd, buff, BUFFER_SIZE)) > 0) {
			write(STDOUT_FILENO, buff, buff_readed);
			memset(buff, 0, BUFFER_SIZE);
		}
	}
	return 0;
}
