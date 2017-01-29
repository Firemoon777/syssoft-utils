#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "lab4.h"
#include "labcheck.h"

static void lab4_print_status(labcheck_error status, char* msg) {
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
	char* cmd = (char*)malloc((15 + strlen(file1) + strlen(file2)) * sizeof(char));
	int result;
	sprintf(cmd, "cmp %s %s 2>/dev/null", file1, file2);
	result = system(cmd);
	free(cmd);
	return WEXITSTATUS(result) == 0 ? LABCHECK_ERROR_OK : LABCHECK_ERROR_FAIL;
}

static int lab4_make_cmd(char* cmd, const char* executable, 
				const lab4_input_type input_type,  const char* input,
				const lab4_output_type output_type, const char* output_file) {
	switch(input_type) {
		case LAB4_INPUT_FILE:
			sprintf(cmd, LAB4_RESTRICTION_LIB" %s %s %c %s", executable, input, output_type * '>', output_file);
			return 0;
			
		case LAB4_INPUT_PIPE:
			sprintf(cmd, "cat %s | "LAB4_RESTRICTION_LIB" %s %c %s", input, executable, output_type * '>', output_file);
			return 0;
		
		default:
			return 1;
	}
}
					

static labcheck_error lab4_check_test(const char* executable, const char* original, 
				const lab4_input_type input_type, const char* input_file, 
				const lab4_output_type output_type) {
	const char* out_file1 = "ex.out", 
	           *out_file2 = "orig.out";
	size_t len = strlen(executable) + strlen(input_file) + strlen(out_file2) + strlen(out_file2) + strlen(LAB4_RESTRICTION_LIB) + 25;
	char* cmd = (char*)malloc(len * sizeof(char));
	int result;
	assert(lab4_make_cmd(cmd, executable, input_type, input_file, output_type, out_file1) == 0);
	result = system(cmd);
	if(result != 0) {
		return LABCHECK_ERROR_CRITICAL;
	}
	
	assert(lab4_make_cmd(cmd, original, input_type, input_file, output_type, out_file2) == 0);
	result = system(cmd);
	if(result != 0) {
		return LABCHECK_ERROR_CRITICAL;
	}
	
	free(cmd);
	
	return lab4_cmp_files(out_file1, out_file2);
}

static int lab4_check_common_tests(const char* executable, const char* original, const lab4_input_type input_type, const lab4_output_type output_type) {
	labcheck_error err;
	int result = 0, i;
	char msg[255], file[1024];
	const int lab4_common_test_count = 4;
	for(i = 0; i < lab4_common_test_count; i++) {
		sprintf(msg, "Checking common test #%i...", i);
		sprintf(file, "%s%i.in", LAB4_TESTS_FOLDER, i);
		err = lab4_check_test(executable, "cat", input_type, file, output_type);
		lab4_print_status(err, msg);
	}
	return result;
}

static labcheck_error lab4_check_restricted_action(const char* executable) {
	char cmd[1024];
	int result;
	sprintf(cmd, "ls -l / | "LAB4_RESTRICTION_LIB" %s >/dev/null", executable);
	result = system(cmd);
	if(WEXITSTATUS(result) == LABCHECK_RESTRICTED_OP_EXITCODE) {
		return LABCHECK_ERROR_FAIL;
	}
	return LABCHECK_ERROR_OK;
}

static int lab4_check_cat(const char* executable) {
	int result = 0;
	labcheck_error err = lab4_check_restricted_action(executable);
	lab4_print_status(err, "Checking for syscalls read/write...");
	if(err != LABCHECK_ERROR_OK) {
		return 1;
	}
	result += lab4_check_common_tests(executable, "cat", LAB4_INPUT_FILE, LAB4_OUTPUT_STDOUT);
	err = lab4_check_test(executable, "cat", LAB4_INPUT_PIPE, LAB4_TESTS_FOLDER"1.in", LAB4_OUTPUT_STDOUT);
	lab4_print_status(err, "Checking for pipe input...");
	
	err = lab4_check_test(executable, "cat", LAB4_INPUT_FILE, LAB4_TESTS_FOLDER"1.in "LAB4_TESTS_FOLDER"2.in "LAB4_TESTS_FOLDER"3.in ", LAB4_OUTPUT_STDOUT);
	lab4_print_status(err, "Checking for several files input... (optional)");
	return result;
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
