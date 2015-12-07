#ifndef		_INCLUDE_H
#define		_INCLUDE_H

#include <apue.h>
#include <errno.h>
#include <signal.h>

//////////////////////////////////////////////////////////////////////////
/*
err_sys: API调用失败，打印errno
err_ret: 错误并打印errno
err_msg: 错误不打印errno
*/

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)		(sizeof(a)/sizeof(*a))
#endif // !ARRAY_SIZE


#endif