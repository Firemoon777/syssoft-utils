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
	int fd, k = 0;
	ssize_t buff_readed, buff_w, i;
	off_t offset;
	
	struct stat st;

	if(argc == 1) {
		fd = STDIN_FILENO;
	} else {
		fd = open(argv[1], O_RDONLY);
	}
	if(fd < 0) {
		perror(argv[1]);	
		return 1;
	}
	
	stat(argv[1], &st);
	offset = st.st_size - BUFFER_SIZE;
		
	while(offset >= 0) {
		buff_readed = pread(fd, buff, BUFFER_SIZE, offset);
		if(buff_readed < 0) {
			perror("read");
			break;
		}
		for(i = buff_readed - 1; i >= 0; i--) {
			if(buff[i] == '\n') {
				k++;
			}
			if(k > 10) {
				offset = offset + i + 1;
				break;
			}
		}
		if(k > 10) {
			break;
		}
		offset -= BUFFER_SIZE;
		if(offset == -BUFFER_SIZE) {
			break;
		} else if(offset < 0) {
			offset = 0;
		}
	}
	if(offset < 0) {
		offset = 0;
	}
	
	while((buff_readed = pread(fd, buff, BUFFER_SIZE, offset)) != 0) {
		if(buff_readed < 0) {
			break;
		}
		buff_w = write(STDOUT_FILENO, buff, buff_readed);
		if(buff_w < 0) {
			perror("write");
			break;
		}
		offset += BUFFER_SIZE;
	}
	return 0;
}
