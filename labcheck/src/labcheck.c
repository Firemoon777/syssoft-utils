#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "labcheck.h"
#include "lab4.h"

static void labcheck_usage(char* name) {
	fprintf(stderr, 
		"usage: %s labnum labvar executable\n", name
		);
}

static int labcheck_start_test(long labnum, long labvar, const char* executable) {
	switch(labnum) {
		case 4:
			return lab4_check(labvar, executable);
		case 5:
			fprintf(stderr, "Lab5 check not implemented\n");
			break;
		case 6:
			fprintf(stderr, "Lab5 check not implemented\n");
			break;
		default:
			fprintf(stderr, "Unknown lab number\n");
			break;
	}
	return -1;
}

int main(int argc, char **argv) {
	unsigned long labvar, labnum;
	int result;
	char *ptr = NULL;
	if(argc != 4) {
		labcheck_usage(argv[0]);
		return -1;
	}
	labnum = strtoul(argv[1], &ptr, 10);
	labvar = strtoul(argv[2], &ptr, 10);
	result = labcheck_start_test(labnum, labvar, argv[3]);
	if(result == 0) {
		printf("Well done!\n");
	} else if(result > 0) {
		printf("You should work harder!\n");
	}
	return result;
}

