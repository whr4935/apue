#ifndef		_UTILS_H_
#define		_UTILS_H_

//#define _POSIX_C_SOURCE 200809L
//#define _XOPEN_SOURCE 700

#ifdef __cplusplus
extern "C" {
#endif

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/syscall.h>

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

/*
 *  Default file access permissions for new files.
 */
#define FILE_MODE   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
 *Default permissions for new directories.
 */
#define DIR_MODE    (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

#define		RT_SUCCESS		0
#define		RT_FAILURE		(-1)

#define MAXLINE 4096            /* max line length */

#define  gettid() syscall(__NR_gettid)
#define msleep(x) usleep(x*1000)//x should be less than 1000, or else use sleep

#define BEGIN_MODULE    printf("========= begin %s ==========\n", __FUNCTION__)
#define END_MODULE    printf("========== end %s ==========\n", __FUNCTION__)
#define TRACE(fmt, arg...) printf("[%s:%d] " fmt, ##arg)

typedef struct {
    unsigned long long begin_time;
    unsigned long long end_time;
    unsigned long long diff;
}BenchMark;

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

void benchmark_begin(BenchMark *bench);
void benchmark_end(BenchMark *bench, const char *fmt, ...);
void print_array(int a[], int N);
void print_index_array(int a[], int p[], int N);


void    err_msg(const char *, ...);         /* {App misc_source} */
void    err_dump(const char *, ...) __attribute__((noreturn));
void    err_quit(const char *, ...) __attribute__((noreturn));
void    err_cont(int, const char *, ...);
void    err_exit(int, const char *, ...) __attribute__((noreturn));
void    err_ret(const char *, ...);
void    err_sys(const char *, ...) __attribute__((noreturn));

void    log_msg(const char *, ...);         /* {App misc_source} */
void    log_open(const char *, int, int);
void    log_quit(const char *, ...) __attribute__((noreturn));
void    log_ret(const char *, ...);
void    log_sys(const char *, ...) __attribute__((noreturn));
void    log_exit(int, const char *, ...) __attribute__((noreturn));

int     lock_reg(int, int, int, off_t, int, off_t); /* {Prog lockreg} */

#define read_lock(fd, offset, whence, len) \
                lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_lock(fd, offset, whence, len) \
                lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd, offset, whence, len) \
                lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_lock(fd, offset, whence, len) \
                lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define un_lock(fd, offset, whence, len) \
                lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

pid_t   lock_test(int, int, off_t, int, off_t);     /* {Prog locktest} */

#define is_read_lockable(fd, offset, whence, len) \
                (lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)
#define is_write_lockable(fd, offset, whence, len) \
                (lock_test((fd), F_WRLCK, (offset), (whence), (len)) == 0)


ssize_t  readn(int, void *, size_t);        /* {Prog readn_writen} */
ssize_t  writen(int, const void *, size_t); /* {Prog readn_writen} */


void     daemonize(const char *);           /* {Prog daemoninit} */

void     tty_atexit(void);                  /* {Prog raw} */


int      ptym_open(char *, int);            /* {Prog ptyopen} */
int      ptys_open(char *);                 /* {Prog ptyopen} */

int      tty_cbreak(int);                   /* {Prog raw} */
int      tty_raw(int);                      /* {Prog raw} */
int      tty_reset(int);                    /* {Prog raw} */
void     tty_atexit(void);                  /* {Prog raw} */
struct termios  *tty_termios(void);         /* {Prog raw} */


int      set_cloexec(int);                  /* {Prog setfd} */








#ifdef __cplusplus
}
#endif

#endif
