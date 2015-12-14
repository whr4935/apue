#include "process.h"
#include <includes.h>
#include <setjmp.h>
#include <sys/resource.h>
#include <pwd.h>
#include <syslog.h>

int TEST_process()
{
	//test_env();
	//test_setjump();
	//test_rlimit();
	//test_fork();
	//test_vfork();
	//test_abort_core();
	//test_segement_fault();
	//test_signal();
	//test_signal_noreentrant();
	//test_sleep1();
	//test_alarm();
	//test_sigprocmask();
	//test_process_group();
	//test_daemon();
	test_unblock_write();

	return 0;
}

static int test_env_routine()
{
	char env_string[20] = "WHR=wang houren";
	char *env_string2 = "WHR=wang houren";
	char *p_env;
	int ret;

	ret = putenv(env_string2);

	p_env = getenv("WHR");
	if (p_env == NULL)
		err_msg("getenv WHR failed!\n");
	else {
		puts(p_env);
	}

	return 0;
}

int test_env()
{
	char **p = environ;
	int ret;
	char *p_env;
	
//////////////////////////////////////////////////////////////////////////
	test_env_routine();
	p_env = getenv("WHR");
	if (p_env == NULL)
		err_msg("getenv WHR failed!\n");
	else {
		puts(p_env);
	}



	puts("==================");

	p = environ;
	while (*p) {
		puts(*p++);
	}

	return 0;
}

static jmp_buf j_buf;

static setjump_routine()
{
	static int count = 0;
	int ret = 0;

	sleep(1);
	longjmp(j_buf, count++);

	return ret;
}

int test_setjump()
{
	int ret;

	ret = setjmp(j_buf);
	if (ret != 0) {
		err_msg("setjmp ret: %d", ret);
	}

	setjump_routine();

	return 0;
}

int test_rlimit()
{
	//struct rlimit r_lim;
	int ret;
	int ret2;

	{
		int block_i = 0;

		block_i++;
	}
//	printf("block_i = %d\n", block_i);


// 	ret = getrlimit(RLIMIT_AS, &r_lim);
// 	if (ret < 0)
// 		err_sys("getrlimit");
// 
// 	printf("total memory: %#x\n", r_lim.rlim_cur);

	return 0;
}

static int globval = 6;
static char fork_buf[] = "a write  to stdout\n";

static void sig_handler(int sig)
{
	printf("receive signal: %d, parent pid=%d\n", sig, getpid());
}

int test_fork()
{
	int var;
	pid_t pid;

	var = 88;
	if (write(STDOUT_FILENO, fork_buf, sizeof(fork_buf)-1) != sizeof(fork_buf)-1)
		err_sys("write");

	signal(SIGCHLD, sig_handler);


	printf("before fork\n");
	if ((pid = fork()) < 0)
		err_sys("fork");
	else if (pid == 0) {
		printf("child pid = %d\n", getpid());
		globval++;
		var++;
		exit(0);
	} else {
		sleep(5);
	}

	printf("pid = %ld, globval = %d, var = %d\n", (long)getpid(), globval, var);
	
	exit(0);
}

int test_vfork()
{
	int var;
	pid_t pid;

	var = 88;
	if (write(STDOUT_FILENO, fork_buf, sizeof(fork_buf) - 1) != sizeof(fork_buf) - 1)
		err_sys("write");

	printf("before vfork\n");

	if ((pid = vfork()) < 0)
		err_sys("fork");
	else if (pid == 0) {
		globval++;
		var++;
		sleep(2);
//		exit(0);
	}
	printf("pid = %d, globval = %d, var = %d\n", (long)getpid(), globval, var);

	exit(0);
	return 0;
}

static int gen_core()
{
	struct rlimit r_lim;
	int ret;

	ret = getrlimit(RLIMIT_CORE, &r_lim);
	if (ret < 0)
		err_sys("getrlimit");
	printf("core limit: %u, rlimit_max = %lu\n", r_lim.rlim_cur, r_lim.rlim_max);

	r_lim.rlim_cur = RLIM_INFINITY; //r_lim.rlim_max;

	ret = setrlimit(RLIMIT_CORE, &r_lim);
	if (ret < 0)
		err_sys("setrlimit");

	ret = getrlimit(RLIMIT_CORE, &r_lim);
	if (ret < 0)
		err_sys("getrlimit");
	printf("core limit: %u, rlimit_max = %u\n", r_lim.rlim_cur, r_lim.rlim_max);

//	printf("generate core...\n");

	return 0;
}

int test_abort_core()
{
	gen_core();

	abort();

	return 0;
}

int test_segement_fault()
{
	int ret;
	char *p = NULL;

	gen_core();

	*p = 88;


	return 0;
}

static void sig_usr(int signo)
{
	if (signo == SIGUSR1) {
		printf("catch SIGUSR1\n");
	} else if (signo == SIGUSR2) {
		printf("catch SIGUSR2\n");
	} else {
		printf("received signo = %d\n", signo);
	}
}

int test_signal()
{
	int ret;

	if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
		err_sys("signal");
	}

	if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
		err_sys("signal");
	}

	for (;;)
		pause();

	return 0;
}

static void my_alarm(int signo)
{
	struct passwd *pwptr;

	printf("in signal handler\n");
	pwptr = getpwnam("root");
	if (pwptr == NULL)
		err_sys("getpwnam");

	alarm(1);
}

int test_signal_noreentrant()
{
	struct passwd *ptr;
	
	signal(SIGALRM, my_alarm);
	alarm(1);

	for (;;) {
		if ((ptr = getpwnam("software")) == NULL)
			err_sys("getpwnam");
		if (strcmp(ptr->pw_name, "software") != 0)
			printf("return value corrupted!, pwname = %s\n", ptr->pw_name);
	}
}

static void sleep1_sig_alrm(int signo)
{

}

static void sleep1_sig_interrupt(int signo)
{
	sigset_t s_set;
	sigprocmask(0, NULL, &s_set);

	if (sigismember(&s_set, signo))
		printf("in sig handler, current signal is blocked!\n");

	printf("catch SIGINT!\n");
}
#define	sig_interrupt_handler sleep1_sig_interrupt

static unsigned int sleep1(unsigned int seconds)
{
	if (signal(SIGALRM,	sleep1_sig_alrm) ==	SIG_ERR) {
		return seconds;
	}

	alarm(seconds);
	pause();
	return alarm(0);
}

int test_sleep1()
{
	int ret;
	time_t beg, end;

	signal(SIGINT, sleep1_sig_interrupt);

	beg = time(NULL);
	ret = sleep1(8);
	printf("sleep left time is %d\n", ret);
	end = time(NULL);

	printf("duration %d\n", end - beg);

	return 0;
}

static jmp_buf env_alrm;
static sleep2_sig_alr(int signo)
{
	longjmp(env_alrm, 1);
}

unsigned int sleep2(unsigned int seconds)
{
	if (seconds == 0)
		return 0;

	if (signal(SIGALRM, sleep1_sig_alrm) == SIG_ERR)
		return seconds;

	if (setjmp(env_alrm) == 0) {
		alarm(seconds);
		pause();
	}

	return alarm(0);
}

int test_sleep2()
{
	int ret;
	time_t beg, end;

	beg = time(NULL);
	ret = sleep2(1);
	end = time(NULL);

	printf("sleep2 duration %d\n", end - beg);

	return 0;
}

static void alarm_handler(int signo)
{

}

static int check_signal_ret(int signo, sighandler_t sig_ret)
{
	if (sig_ret == SIG_IGN)
		printf("old %s handler is SIG_IGN!\n", strsignal(signo));
	else if (sig_ret == SIG_DFL)
		printf("old %s handler is SIG_DFL!\n", strsignal(signo));
	else
		printf("old %s handler is user defined handler!\n", strsignal(signo));

	return 0;
}

int test_alarm()
{
	int ret;
	sighandler_t sig_ret;

	if ((sig_ret = signal(SIGALRM, alarm_handler)) == SIG_ERR)
		err_sys("signal");

	check_signal_ret(SIGALRM, sig_ret);

	errno = 0;
	ret = alarm(2);
	printf("ret = %d\n", ret);

	ret = alarm(0);
	printf("ret = %d\n", ret);
	sig_ret = signal(SIGALRM, SIG_DFL);
	check_signal_ret(SIGALRM, sig_ret);

	return 0;
}

static void sig_quit_handler(int signo)
{
	printf("catch SIGQUIT\n");

	sigset_t s_set;
	sigprocmask(0, NULL, &s_set);

	if (sigismember(&s_set, signo))
		printf("in sig handler, current signal is blocked!\n");

// 	if (signal(SIGQUIT, SIG_DFL) < 0)
// 		err_sys("can't resotre SIGQUIT handler");
}

int test_sigprocmask()
{
	sigset_t s_set, s_oldset;
	int ret;

	signal(SIGINT, sig_interrupt_handler);
	signal(SIGQUIT, sig_quit_handler);

	gen_core();

	sigemptyset(&s_set);
	sigaddset(&s_set, SIGQUIT);

	ret = sigprocmask(SIG_BLOCK, &s_set, &s_oldset);
	if (ret < 0)
		err_sys("sigprocmask");

	sleep(5);

	if (sigpending(&s_set) < 0)
		err_sys("sigpending error");
	if (sigismember(&s_set, SIGQUIT))
		printf("\n SIGQUIT is pending\n");

	ret = sigprocmask(SIG_SETMASK, &s_oldset, NULL);
	if (ret < 0)
		err_sys("sigprocmask");	
	printf("SIGQUIT unblocked!\n");

	sleep(5);
	for (;;)
		pause();

	return 0;
}

int test_process_group()
{
	pid_t pgp;
	pid_t pgp_2;
	pid_t pgp_3;
	pid_t pid;
	pid_t child_pid;
	pid_t sid;
	int ret;

	pgp = getpgrp();
	pgp_2 = getpgid(getpid());
	pgp_3 = getpgid(0);
	pid = getpid();

	child_pid = fork();
	if (child_pid < 0)
		err_sys("fork");
	else if (child_pid == 0) {
		pgp = getpgrp();
		printf("child process group id = %d, pid = %d\n", pgp, getpid());

		setpgid(0, 0);

		printf("after setpgid, child process group id = %d, pid = %d\n", getpgid(0), getpid());

		exit(0);
	} else if (child_pid > 0) {
		printf("parent process group id = %d\n", getpgrp());
		printf("parent session ID is %d\n", getsid(0));

		sid = getsid(0);
		
		int fd;
		char buf[100];
		int len;
		char *ptr = buf;
		int chunk;

		sprintf(buf, "/proc/%d/cmdline", sid);
		fd = open(buf, O_RDONLY);
		if (fd < 0)
			err_sys("open");

		ptr = buf;
		len = 50;
		memset(buf, 0, len);

		while (len > 0) {
			ret = read(fd, ptr, 1);
			if (ret < 0) {
				if (errno == EINTR)
					ret = 0;
				else {
					err_msg("read");
					break;
				}
			} else if (ret == 0) {
				printf("read end of file!\n");
				break;
			}

			ptr += ret;
			len -= ret;
		}
		close(fd);

		printf("session id cmdline: %s\n", buf);
	}

	return 0;
}

int test_daemon()
{
	int i, fd0, fd1, fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;
	int cnt = 0;

	umask(0);

	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
		err_quit("can't get file descriptor limit");

	if ((pid = fork()) < 0)
		err_sys("fork");
	else if (pid != 0)
		exit(0);

	setsid();

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		err_sys("can't ignore SIGHUP");

	if ((pid = fork()) < 0)
		err_sys("fork");
	else if (pid != 0)
		exit(0);

// 	if (chdir("/") < 0)
// 		err_sys("chdir");

	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;

	for (i = 0; i < rl.rlim_max; ++i)
		close(i);

	openlog("apue", LOG_CONS, LOG_USER);
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(fd0);
	fd2 = dup(fd0);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR, "invalid file descriptor!\n");
		exit(1);
	}

	syslog(LOG_INFO, "apue loop start...");
	for (;;) {
		sleep(1);

		++cnt;
		syslog(LOG_INFO, "apue count %d", cnt);
		if (cnt >= 20) {
			syslog(LOG_INFO, "cnt = %d, apue daemon ready exit...", cnt);
			break;
		}
	}

	closelog();
	return 0;
}

int test_single_instance_daemon()
{
	//lockfile();
}

int test_unblock_write()
{
	static char buf[500000];
	int val;
	int ret;
	int ntowrite, nwrite;
	char *ptr;

	ntowrite = read(STDIN_FILENO, buf, ARRAY_SIZE(buf));
	fprintf(stderr, "read %d bytes\n", ntowrite);

	val = fcntl(STDOUT_FILENO, F_GETFL, 0);
	if (val < 0)
		err_sys("fcntl");

	fprintf(stderr, "val = %#x\n", val);

	switch (val & O_ACCMODE) {
	case O_RDONLY:
		fprintf(stderr, "read only ");
		break;

	case O_WRONLY:
		fprintf(stderr, "write only ");
		break;

	case O_RDWR:
		fprintf(stderr, "read write ");
		break;
	}

	if (val & O_APPEND)
		fprintf(stderr, ", append");

	if (val & O_NONBLOCK)
		fprintf(stderr, ", noblocking");

	if (val & O_SYNC)
		fprintf(stderr, ", synchronous writes");

	fputc('\n', stderr);
//	fputs("\n", stderr);

	val |= O_NONBLOCK;

	ret = fcntl(STDOUT_FILENO, F_SETFL, val);
	if (ret < 0)
		err_sys("fcntl");

	ptr = buf;
	while (ntowrite > 0) {
		nwrite = write(STDOUT_FILENO, ptr, ntowrite);
		fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);

		if (nwrite > 0) {
			ptr += nwrite;
			ntowrite -= nwrite;
		}
	}
	
	return 0;
}