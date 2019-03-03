#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

//////////////////////////////////////////////////////////////////////////
#define		DBG_TYPE		0

#if	DBG_TYPE==0
#define		DATA_SIZE		10
#define		DEBUG_PRINT		1
#elif DBG_TYPE==1
#define		DATA_SIZE		1000000
#define		DEBUG_PRINT		0
#endif

#if DEBUG_PRINT
#	define		log		printf
#else
#	define		log
#endif

//////////////////////////////////////////////////////////////////////////
typedef enum {
    FAILURE = -1,
    SUCCESS,
}STATUS;

typedef enum {
    FALSE,
    TRUE,
}BOOL;

typedef		int				element_type;

//////////////////////////////////////////////////////////////////////////
#define  ARRAY_SIZE(a) (sizeof(a)/sizeof(*a))

/* #if _MSC_VER <= 1800 */
/* #	define	inline		__inline */
/* #endif */

#define TEST_SUITE_ENTRY(entry)                 \
    {#entry, NULL, NULL, NULL, NULL, entry}

#define TEST_CASE_ENTRY(entry) \
    {#entry, entry}

//////////////////////////////////////////////////////////////////////////
//util function
int initUtil();
int uninitUtil();
static int getRandom()
{
    return rand();
}
int printArry(element_type *e, size_t size);
int statisticTimeFunc(int(*pf)(void *), void *arg, char *fmt, ...);

#define swap(a, b) {element_type t = (a); (a) = (b); (b) = (t);}

//////////////////////////////////////////////////////////////////////////


#endif
