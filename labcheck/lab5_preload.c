#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define EXITCODE 42

int system(const char* string) {
	return 666;
}

FILE* popen(const char* command, const char* mode) {
	_exit(EXITCODE);
	return NULL;
}
