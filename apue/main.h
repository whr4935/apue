#ifndef		_MAIN_H
#define		_MAIN_H

#include <includes.h>
#include "dir/file_dir.h"
#include "misc/misc.h"


//////////////////////////////////////////////////////////////////////////
#define		TEST_FILE_DIR	1
#define		TEST_MISC		0


int (*test_func_array[])() = {
#if TEST_FILE_DIR
	TEST_file_dir,
#endif

#if TEST_MISC
	TEST_misc,
#endif
};



#endif