#include <stdio.h>
#include <stdlib.h>
#include "../labcheck.h"

int scanf(const char *format, ...) {
	exit(LABCHECK_RESTRICTED_OP_EXITCODE);
}

int printf(const char *format, ...) {
	exit(LABCHECK_RESTRICTED_OP_EXITCODE);
}
