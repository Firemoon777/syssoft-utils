#include <unistd.h>
#include <stdio.h>

#define EXITCODE 42

int scanf(const char *format, ...) {
	_exit(EXITCODE);
}

int fscanf(FILE *stream, const char *format, ...) {
	_exit(EXITCODE);
}

int sscanf(const char *str, const char *format, ...) {
	_exit(EXITCODE);
}

int printf(const char *format, ...) {
	_exit(EXITCODE);
}

int fprintf(FILE *stream, const char *format, ...) {
	_exit(EXITCODE);
}

int dprintf(int fd, const char *format, ...) {
	_exit(EXITCODE);
}

int puts(const char *s) {
	_exit(EXITCODE);
}

size_t fread(void *ptr, size_t  size,  size_t  nitems,  FILE *stream) {
	_exit(EXITCODE);
}

FILE *fopen(const char *filename, const char *mode) {
	_exit(EXITCODE);
}

int fclose(FILE *stream) {
	_exit(EXITCODE);
}

/*perror*/

size_t fwrite(const void *ptr, size_t size,  size_t  nitems, FILE *stream) {
	_exit(EXITCODE);
}
