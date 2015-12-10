#ifndef		_INCLUDE_H
#define		_INCLUDE_H

#include <apue.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////
/*
err_sys: API调用失败，打印errno
err_ret: 错误并打印errno
err_msg: 错误不打印errno
*/

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)		(sizeof(a)/sizeof(*a))
#endif // !ARRAY_SIZE

#define		RT_SUCCESS		0
#define		RT_FAILURE		(-1)

#define  gettid() syscall(__NR_gettid)
#define msleep(x) usleep(x*1000)//x should be less than 1000, or else use sleep

//////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
#define LOG_INFO(format,...)  \
	do{ \
	fprintf(stderr,TERMINAL_COLOR_GREEN"[INFO:%s -> %s: %d] " format TERMINAL_COLOR_TERRESET, __FILE__, \
	__func__, __LINE__, ##__VA_ARGS__ ); \
			}while(0);

#define LOG_DBG(format,...)  \
	do{ \
	fprintf(stderr, TERMINAL_COLOR_CYAN"[DEBUG:%s -> %s : %d] " format TERMINAL_COLOR_TERRESET, __FILE__, \
	__func__, __LINE__, ##__VA_ARGS__ ); \
			}while(0);

#define LOG_WAR(var,...)	\
	do{	\
	fprintf(stderr, TERMINAL_COLOR_BOLDMAGENTA"[WARING:%s -> %s: %d] " var TERMINAL_COLOR_TERRESET, __FILE__, \
	__func__,__LINE__, ##__VA_ARGS__);	\
			}while(0);

#define LOG_ERR(var,...) 	\
	do{	\
	fprintf(stderr, TERMINAL_COLOR_BOLDRED"[ERROR:%s -> %s: %d] " var TERMINAL_COLOR_TERRESET, __FILE__, \
	__func__,__LINE__, ##__VA_ARGS__); \
			}while(0);

#else
#define LOG_INFO(format,...)  \
	do{ \
	fprintf(stderr,TERMINAL_COLOR_GREEN format TERMINAL_COLOR_TERRESET, ##__VA_ARGS__ ); \
			}while(0);

#define LOG_DBG(format,...)  

#define LOG_WAR(var,...)	\
	do{	\
	fprintf(stderr,TERMINAL_COLOR_BOLDMAGENTA var TERMINAL_COLOR_TERRESET, ##__VA_ARGS__);	\
			}while(0);

#define LOG_ERR(var,...) 	\
	do{	\
	fprintf(stderr,TERMINAL_COLOR_BOLDRED var TERMINAL_COLOR_TERRESET, ##__VA_ARGS__); \
			}while(0);

#endif


//the following are UBUNTU/LINUX ONLY terminal color codes.
#define		TERMINAL_COLOR_TERRESET		"\033[0m"
#define		TERMINAL_COLOR_BLACK		"\033[30m" /* Black */
#define		TERMINAL_COLOR_RED			"\033[31m" /* Red */
#define		TERMINAL_COLOR_GREEN		"\033[32m" /* Green */
#define		TERMINAL_COLOR_YELLOW		"\033[33m" /* Yellow */
#define		TERMINAL_COLOR_BLUE			"\033[34m" /* Blue */
#define		TERMINAL_COLOR_MAGENTA		"\033[35m" /* Magenta */
#define		TERMINAL_COLOR_CYAN			"\033[36m" /* Cyan */
#define		TERMINAL_COLOR_WHITE		"\033[37m" /* White */
#define		TERMINAL_COLOR_BOLDBLACK	"\033[1m\033[30m" /* Bold Black */
#define		TERMINAL_COLOR_BOLDRED		"\033[1m\033[31m" /* Bold Red */
#define		TERMINAL_COLOR_BOLDGREEN	"\033[1m\033[32m" /* Bold Green */
#define		TERMINAL_COLOR_BOLDYELLOW	"\033[1m\033[33m" /* Bold Yellow */
#define		TERMINAL_COLOR_BOLDBLUE		"\033[1m\033[34m" /* Bold Blue */
#define		TERMINAL_COLOR_BOLDMAGENTA	"\033[1m\033[35m" /* Bold Magenta */
#define		TERMINAL_COLOR_BOLDCYAN		"\033[1m\033[36m" /* Bold Cyan */
#define		TERMINAL_COLOR_BOLDWHITE	"\033[1m\033[37m" /* Bold White */

//////////////////////////////////////////////////////////////////////////



#endif