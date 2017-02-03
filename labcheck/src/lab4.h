#ifndef _H_LABCHECK_LAB4_
#define _H_LABCHECK_LAB4_

int lab4_check(unsigned long var, const char* executable);

#define LAB4_VARIANTS_COUNT 15
#define LAB4_TESTS_FOLDER LABCHECK_TESTS_FOLDER"lab4/"
#define LAB4_RESTRICTION_LIB "LD_PRELOAD=./bin/librestriction.so"

#define CHECK_SEVERAL_FILES 1
#define CHECK_PIPE_INPUT 2

typedef enum {
	LAB4_ZSH  =  0,
	LAB4_CAT  =  1,
	LAB4_CP   =  2,
	LAB4_HEAD =  3,
	LAB4_TAIL =  4,
	LAB4_TEE  =  5,
	LAB4_WC   =  6,
	LAB4_CMP  =  7,
	LAB4_MORE =  8,
	LAB4_LESS =  9,
	LAB4_SED  = 10,
	LAB4_AWK  = 11,
	LAB4_SH   = 12,
	LAB4_KSH  = 13,
	LAB4_CSH  = 14
} lab4_labvar;

typedef enum { 
	LAB4_INPUT_FILE = 0,
	LAB4_INPUT_PIPE = 1
} lab4_input_type;

typedef enum { 
	LAB4_OUTPUT_FILE = 0,
	LAB4_OUTPUT_STDOUT = 1
} lab4_output_type;

#endif
