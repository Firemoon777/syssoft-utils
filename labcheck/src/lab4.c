#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "lab4.h"
#include "labcheck.h"

static void lab4_check_print_status(labcheck_error status, char* msg) {
	switch(status) {
		case LABCHECK_ERROR_OK:
			fprintf(stderr, "[  OK  ] %s\n", msg);
			break;
		case LABCHECK_ERROR_FAIL:
			fprintf(stderr, "[ FAIL ] %s\n", msg);
			break;
		case LABCHECK_ERROR_CRITICAL:
			fprintf(stderr, "[CRITICAL] %s\n", msg);
			exit(1);
	}
}

static int lab4_cmp_files(const char* file1, const char* file2) {
	char* cmd = (char*)malloc((5 + strlen(file1) + strlen(file2)) * sizeof(char));
	int result;
	sprintf(cmd, "cmp %s %s", file1, file2);
	result = system(cmd);
	free(cmd);
	return WEXITSTATUS(result) == 0 ? LABCHECK_ERROR_OK : LABCHECK_ERROR_FAIL;
}

static labcheck_error lab4_check_test(const char* executable, const char* original, 
				const char* input_files, const int redirect_needed) {
	const char* out_file1 = "ex.out", 
	           *out_file2 = "orig.out";
	size_t len = strlen(executable) + strlen(input_files) + strlen(out_file2) + strlen(out_file2) + 25;
	char* cmd = (char*)malloc(len * sizeof(char));
	int result;
	sprintf(cmd, "%s %s %c %s", executable, input_files, redirect_needed * '>', out_file1);
	/*printf("cmd to execute: %s\n", cmd);*/
	result = system(cmd);
	if(result != 0) {
		return LABCHECK_ERROR_CRITICAL;
	}
	
	sprintf(cmd, "%s %s %c %s", original, input_files, redirect_needed * '>', out_file2);
	/*printf("cmd to execute: %s\n", cmd);*/
	result = system(cmd);
	if(result != 0) {
		return LABCHECK_ERROR_CRITICAL;
	}
	
	free(cmd);
	
	return lab4_cmp_files(out_file1, out_file2);
}

static labcheck_error lab4_check_restricted_action(const char* executable) {
	char cmd[1024];
	int result;
	sprintf(cmd, "ls -l / | LD_PRELOAD=./bin/librestriction.so %s >/dev/null", executable);
	result = system(cmd);
	if(WEXITSTATUS(result) == LABCHECK_RESTRICTED_OP_EXITCODE) {
		return LABCHECK_ERROR_FAIL;
	}
	return LABCHECK_ERROR_OK;
}

static int lab4_check_cat(const char* executable) {
	labcheck_error err = lab4_check_restricted_action(executable);
	lab4_check_print_status(err, "Checking for syscalls read/write...");
	if(err != LABCHECK_ERROR_OK) {
		return 1;
	}
	err = lab4_check_test(executable, "cat", LAB4_TESTS_FOLDER"1.in", 1);
	lab4_check_print_status(err, "Checking common tests...");
	return 0;
}

int lab4_check(unsigned long var, const char* executable) {
	const char* var_name[] = {
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
