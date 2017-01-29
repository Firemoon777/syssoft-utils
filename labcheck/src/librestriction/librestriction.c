#include <unistd.h>
#include "../labcheck.h"

int scanf(const char *format, ...) {
	_exit(LABCHECK_RESTRICTED_OP_EXITCODE);
}

/*int fscanf(FILE *stream, const char *format, ...) {
	_exit(LABCHECK_RESTRICTED_OP_EXITCODE);
}

int sscanf(const char *str, const char *format, ...) {
	_exit(LABCHECK_RESTRICTED_OP_EXITCODE);
}*/

int printf(const char *format, ...) {
	_exit(LABCHECK_RESTRICTED_OP_EXITCODE);
}

/*int fprintf(FILE *stream, const char *format, ...) {
	exit(LABCHECK_RESTRICTED_OP_EXITCODE);
}

int dprintf(int fd, const char *format, ...) {
	exit(LABCHECK_RESTRICTED_OP_EXITCODE);
}

int sprintf(char *str, const char *format, ...) {
	exit(LABCHECK_RESTRICTED_OP_EXITCODE);
}

int snprintf(char *str, size_t size, const char *format, ...) {
	exit(LABCHECK_RESTRICTED_OP_EXITCODE);
}*/
