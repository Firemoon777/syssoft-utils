#include "lab4.h"
#include <stdio.h>

static const char* var_name[] = {
    "zsh", 
    "cat", 
    "cp", 
    "head", 
    "tail", 
    "tee", 
    "wc", 
    "cmp", 
    "more", 
    "less", 
    "sed", 
    "awk", 
    "sh", 
    "ksh", 
    "csh"
};

static int lab4_check_cat(const char* executable) {
	/*int result = system("LD_PRELOAD=./librestriction.so ./a.out");
	printf("Result = %i\n", WEXITSTATUS(result));*/
	return 0;
}

int lab4_check(unsigned long var, const char* executable) {
	if(var > LAB4_VARIANTS_COUNT) {
		fprintf(stderr, "variant %lu is unavaliable\n", var);
		return -1;
	}
	switch(var) {
		case LAB4_CAT:
			return lab4_check_cat(executable);
			
		default: 
			fprintf(stderr, "checker for %s is not implemented!\n", var_name[var]);
			break;
	}
	return -1;
}
