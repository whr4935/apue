#ifndef		_INCLUDE_H
#define		_INCLUDE_H

#include <apue.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <setjmp.h>
#include <pwd.h>
#include <aio.h>
#include <syslog.h>
#include <poll.h>
#include <semaphore.h>
#include <netdb.h>
#include <termios.h>
#include <pthread.h>

#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/sysinfo.h>

#include <arpa/inet.h>


//////////////////////////////////////////////////////////////////////////
/*
err_sys: 打印errno，退出
err_ret: 打印errno，返回
err_quit:不打印errno，退出
err_msg: 不打印errno，返回
*/

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)		(sizeof(a)/sizeof(*a))
#endif // !ARRAY_SIZE

#define		RT_SUCCESS		0
#define		RT_FAILURE		(-1)

#define  gettid() syscall(__NR_gettid)
#define msleep(x) usleep(x*1000)//x should be less than 1000, or else use sleep

//////////////////////////////////////////////////////////////////////////
//common interfaces
int my_set_cloexec(int fd);
int my_daemonize(char *cmd);
int my_serv_listen(const char *name);
int my_serv_accept(int listenfd, uid_t *uidptr);
int my_cli_conn(const char *name);
int my_send_fd(int fd, int fd_to_send);
int my_send_err(int fd, int status, const char* errmsg);
int my_recv_fd(int fd, ssize_t(*userfunc)(int, const void*, size_t));

//////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
#define log_info(format,...)  \
	do{ \
	fprintf(stderr,TERMINAL_COLOR_GREEN"[INFO:%s -> %s: %d] " format TERMINAL_COLOR_TERRESET, __FILE__, \
	__func__, __LINE__, ##__VA_ARGS__ ); \
			}while(0);

#define log_dbg(format,...)  \
	do{ \
	fprintf(stderr, TERMINAL_COLOR_CYAN"[DEBUG:%s -> %s : %d] " format TERMINAL_COLOR_TERRESET, __FILE__, \
	__func__, __LINE__, ##__VA_ARGS__ ); \
			}while(0);

#define log_war(var,...)	\
	do{	\
	fprintf(stderr, TERMINAL_COLOR_BOLDMAGENTA"[WARING:%s -> %s: %d] " var TERMINAL_COLOR_TERRESET, __FILE__, \
	__func__,__LINE__, ##__VA_ARGS__);	\
			}while(0);

#define log_err(var,...) 	\
	do{	\
	fprintf(stderr, TERMINAL_COLOR_BOLDRED"[ERROR:%s -> %s: %d] " var TERMINAL_COLOR_TERRESET, __FILE__, \
	__func__,__LINE__, ##__VA_ARGS__); \
			}while(0);

#else
#define log_info(format,...)  \
	do{ \
	fprintf(stderr,TERMINAL_COLOR_GREEN format TERMINAL_COLOR_TERRESET, ##__VA_ARGS__ ); \
			}while(0);

#define log_dbg(format,...)  

#define log_war(var,...)	\
	do{	\
	fprintf(stderr,TERMINAL_COLOR_BOLDMAGENTA var TERMINAL_COLOR_TERRESET, ##__VA_ARGS__);	\
			}while(0);

#define log_err(var,...) 	\
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