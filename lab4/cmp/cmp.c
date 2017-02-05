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

int uint_to_string(char* str, size_t n) {
	const int size = 10;
	char num[size];
	int i;
	for(i = 0; i < size; i++) {
		num[i] = ' ';
	}
	num[size-1] = 0;
	i = size-2;
	do {
		num[i] = (n % 10) + '0';
		i--;
		n = n / 10;
	} while(n > 0);
	strcat(str, num + i);
	return size;
}

int main(int argc, char** argv) {
	char buff[2][BUFFER_SIZE];
	int fd[2];
	ssize_t buff_readed[2], min;
	size_t i = 0, c = 1, l = 1;
	
	if(argc != 3) {
		write(STDOUT_FILENO, "Usage: cmp file1 file2\n", 22);  
		return 1;
	}
	
	fd[0] = open(argv[1], O_RDONLY);
	if(fd[0] < 0) {
		perror(argv[1]);
		return errno;
	}
	
	fd[1] = open(argv[2], O_RDONLY);
	if(fd[1] < 0) {
		perror(argv[2]);
		return errno;
	}
	
	buff_readed[0] = 1;
	buff_readed[1] = 0;
	
	while(buff_readed[0] > 0) {
		buff_readed[0] = read(fd[0], buff[0], BUFFER_SIZE);
		buff_readed[1] = read(fd[1], buff[1], BUFFER_SIZE);
		min = (buff_readed[0] < buff_readed[1]) ? buff_readed[0] : buff_readed[1];
		if(buff_readed[0] < 0 || buff_readed[1] < 0) {
			perror("read");
			return -1;
		}
		for(i = 0; i < min; i++) {			
			if(buff[0][i] != buff[1][i]) {
				char str[1000];
				strcpy(str, argv[1]);
				strcat(str, " ");
				strcat(str, argv[2]);
				strcat(str, " differ: char ");
				uint_to_string(str, c);
				strcat(str, ", line ");
				uint_to_string(str, l);
				strcat(str, "\n");
				write(STDOUT_FILENO, str, strlen(str));
				return 1;
			}
			c++;
			if(buff[0][i] == '\n') 
				l++;
		}
		if(buff_readed[0] == min && buff_readed[0] != buff_readed[1]) {
			char str[1000] = "cmp: EOF on ";
			strcat(str, argv[1]);
			strcat(str, "\n");
			write(STDOUT_FILENO, str, strlen(str));
			return 1;
		}
		if(buff_readed[1] == min && buff_readed[0] != buff_readed[1]) {
			char str[1000] = "cmp: EOF on ";
			strcat(str, argv[2]);
			strcat(str, "\n");
			write(STDOUT_FILENO, str, strlen(str));
			return 1;
		}
	}
		
	return 0;
}
