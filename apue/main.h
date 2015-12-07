#ifndef		_MAIN_H
#define		_MAIN_H

#include <includes.h>
#include "dir/file_dir.h"
#include "misc/misc.h"
#include "process/process.h"


//////////////////////////////////////////////////////////////////////////
#define		TEST_FILE_DIR	0
#define		TEST_MISC		0
#define		TEST_PROCESS	1


int (*test_func_array[])() = {
#if TEST_FILE_DIR
	TEST_file_dir,
#endif

#if TEST_MISC
	TEST_misc,
#endif

#if TEST_PROCESS
	TEST_process,
#endif

	NULL,
};



#endif