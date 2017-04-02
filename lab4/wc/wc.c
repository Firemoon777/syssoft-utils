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

static int uint_to_string(char* str, size_t n) {
	const int size = 10;
	char num[size];
	int i;
	for(i = 0; i < size; i++) {
		num[i] = ' ';
	}
	num[size-1] = '\0';
	i = size-2;
	do {
		num[i] = (char)((n % 10) + (int)'0');
		i--;
		n = n / 10;
	} while(n > 0);
	strcat(str, num);
	return size;
}

static void print_ans(size_t l, size_t w, size_t b, const char* name) {
	char output[1000];
	output[0] = '\0';
	
	(void)uint_to_string(output, l);
	(void)uint_to_string(output, w);
	(void)uint_to_string(output, b);
	
	strcat(output, " ");
	strcat(output, name);
	strcat(output, "\n");
	
	(void)write(STDOUT_FILENO, output, strlen(output));
}

static int do_task(const char* filename, size_t* tl, size_t* tw, size_t* tb) {
	char buff[BUFFER_SIZE];
	ssize_t j, buff_readed;
	size_t b = 0, w = 0, l = 0;
	int word = 0, fd;
	if(strcmp(filename, "-") == 0) {
		fd = STDIN_FILENO;
	} else {
		fd = open(filename, O_RDONLY);
	}
	if(fd < 0) {
		perror(filename);	
		return 1;
	}
	while((buff_readed = read(fd, buff, BUFFER_SIZE)) > 0) {
		b += buff_readed;
		for(j = 0; j < buff_readed; j++) {
			if(buff[j] == ' ' || buff[j] == '\t' || buff[j] == '\n') {
				if(word != 0) {
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
		perror(filename);
		return 1;
	}
	*tb += b;
	*tw += w;
	*tl += l;
	print_ans(l, w, b, filename);
	(void)close(fd);
	return 0;
}

int main(int argc, char** argv) {
	int i = 1;
	int exitcode = EXIT_SUCCESS;
	size_t tb = 0, tw = 0, tl = 0;;
	if(argc == 1) {
		do_task("-", &tl, &tw, &tb);
	}
	for(i = 1; i < argc; i++) {
		exitcode += do_task(argv[i], &tl, &tw, &tb);
	}
	if(argc > 2) {
		print_ans(tl, tw, tb, "total");
	}
	return exitcode;
}
