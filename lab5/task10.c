#include <stdio.h>
#include <assert.h>
#include <unistd.h> /* pipe */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFER_SIZE 80

int main(int argc, char** argv) {
	int fd[2];
	int file;
	char buff[2*BUFFER_SIZE], out[BUFFER_SIZE];
	ssize_t buff_readed, i;
	if(argc != 2) {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		return -1;
	}
	if(pipe(fd) < 0) {
		perror("pipe");
		return -1;
	}
	switch(fork()) {
		case -1:
			perror("fork");
			return -2;

		case 0:
			/* Child */
			/* Close unused fd */
			close(fd[0]);
			/* Replace stdin with fd[1] */
			dup2(fd[1], STDIN_FILENO);
			/* */
			/* Start wc */
			execl("/usr/bin/wc", "wc", (char*)0);
			break;

		default:
			/* Parent */
			/* Close unused fd */
			close(fd[1]);
			file = open(argv[1], O_RDONLY);
			if(file < 0) {
				perror(argv[1]);
				return -2;
			}
			while((buff_readed = read(file, buff, 2*BUFFER_SIZE)) > 0) {
				for(i = 0; i < (buff_readed + 1) / 2; i++) {
					out[i] = buff[2*i];
				}	
				write(fd[0], out, (size_t)i);
			}
			if(buff_readed < 0) {
				perror("read");
				return -3;
			}
			close(file);
			close(fd[0]);
			wait(NULL);
			break;
	}
	return 0;
}
