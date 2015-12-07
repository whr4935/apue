#include "process.h"
#include <includes.h>
#include <setjmp.h>
#include <sys/resource.h>

int TEST_process()
{
	//test_env();
	//test_setjump();
	//test_rlimit();
	//test_fork();
	//test_vfork();
	//test_abort_core();
	//test_segement_fault();
	test_signal();

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