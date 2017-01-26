#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	int result = system("LD_PRELOAD=./librestriction.so ./a.out");
	printf("Result = %i\n", WEXITSTATUS(result));
	return 0;
}

