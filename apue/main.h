#ifndef		_MAIN_H
#define		_MAIN_H

#include <includes.h>
#include "dir/file_dir.h"
#include "misc/misc.h"
#include "process/process.h"
#include "process/thread.h"
#include "process/ipc.h"
#include "process/term.h"
#include "db/db_test.h"


//////////////////////////////////////////////////////////////////////////
#define		TEST_FILE_DIR	0
#define		TEST_MISC		1
#define		TEST_PROCESS	0
#define		TEST_THREAD		0
#define		TEST_IPC		0
#define		TEST_TERM		0
#define		TEST_DB			0


int (*test_func_array[])(int argc, char **argv) = {
#if TEST_FILE_DIR
	TEST_file_dir,
#endif

#if TEST_MISC
	TEST_misc,
#endif

#if TEST_PROCESS
	TEST_process,
#endif

#if TEST_THREAD
	TEST_thread,
#endif

#if	TEST_IPC
	TEST_ipc,
#endif

#if TEST_TERM
	TEST_term,
#endif

#if TEST_DB
	TEST_db,
#endif

	NULL,
};



#endif