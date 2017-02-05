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

int sprintf(char *str, const char *format, ...) {
	_exit(EXITCODE);
}

int snprintf(char *str, size_t size, const char *format, ...) {
	_exit(EXITCODE);
}
