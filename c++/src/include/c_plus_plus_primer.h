#ifndef _C_PLUS_PLUS_PRIMER_H
#define _C_PLUS_PLUS_PRIMER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <stdexcept>
#include <cassert>

#include <string>
#include <vector>
#include <iterator>
#include <initializer_list>
#include <list>
#include <forward_list>
#include <deque>
#include <array>
#include <utility>	//for swap
#include <stack>
#include <queue>
#include <numeric>
#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <memory>

//////////////////////////////////////////////////////////////////////////
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::initializer_list;
using namespace std::placeholders;

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

typedef		int				element_type;

//////////////////////////////////////////////////////////////////////////
#define  ARRAY_SIZE(a) (sizeof(a)/sizeof(*a))

#if _MSC_VER <= 1800
#	define	inline		__inline
#endif

//////////////////////////////////////////////////////////////////////////
//util function
int initUtil();
int uninitUtil();
inline int getRandom()
{
    return rand();
}
int printArry(element_type *e, size_t size);
int statisticTimeFunc(int(*pf)(void *), void *arg, char *fmt, ...);


//////////////////////////////////////////////////////////////////////////


#endif
