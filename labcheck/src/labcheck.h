#ifndef _H_LABCHECK_
#define _H_LABCHECK_

#define LABCHECK_RESTRICTED_OP_EXITCODE 255
#define LABCHECK_TESTS_FOLDER "./labtests/"

typedef enum {
	LABCHECK_ERROR_OK = 0, 
	LABCHECK_ERROR_FAIL = 1,
	LABCHECK_ERROR_CRITICAL = 2
} labcheck_error;

#endif
