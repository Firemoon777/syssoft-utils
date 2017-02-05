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
	strcat(str, num);
	return size;
}

void print_ans(size_t l, size_t w, size_t b, const char* name) {
	char output[1000];
	output[0] = 0;
	
	uint_to_string(output, l);
	uint_to_string(output, w);
	uint_to_string(output, b);
	
	strcat(output, " ");
	strcat(output, name);
	strcat(output, "\n");
	
	write(STDOUT_FILENO, output, strlen(output));
}

int main(int argc, char** argv) {
	char buff[BUFFER_SIZE+1];
	int fd, word = 0;
	ssize_t buff_readed;
	size_t i = 1, b = 0, w = 0, l = 0, j, tb = 0, tw = 0, tl = 0;;
	if(argc == 1) {
		argc++;
		*(argv + 1) = "-\0";
	}
	for(i = 1; i < argc; i++) {
		b = 0;
		w = 0;
		l = 0;
		word = 0;
		if(strcmp(argv[i], "-") == 0) {
			fd = STDIN_FILENO;
		} else {
			fd = open(argv[i], O_RDONLY);
		}
		if(fd < 0) {
			perror(argv[i]);	
			continue;
		}
		while((buff_readed = read(fd, buff, BUFFER_SIZE)) > 0) {
			b += buff_readed;
			for(j = 0; j < buff_readed; j++) {
				if(buff[j] == ' ' || buff[j] == '\t' || buff[j] == '\n' || buff[j] == EOF) {
					if(word) {
						w++;
					}
					word = 0;
				} else {
					word = 1;
				}
				if(buff[j] == '\n') 
					l++;
			}
		}
		if(buff_readed < 0) {
			perror(argv[i]);
			continue;
		}
		tb += b;
		tw += w;
		tl += l;
		print_ans(l, w, b, argv[i]);
	}
	if(argc > 2) {
		print_ans(tl, tw, tb, "total");
	}
	return 0;
}
