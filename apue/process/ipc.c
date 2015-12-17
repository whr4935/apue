#include "ipc.h"
#include <includes.h>

//////////////////////////////////////////////////////////////////////////
static int coprocess_add2();

//////////////////////////////////////////////////////////////////////////
int TEST_ipc()
{
	//test_pipe();
	//test_pipe_page();
	//coprocess_add2();
	//test_coprocess();
	//test_msgqueue();
	test_semaphore();

	return 0;
}

int test_pipe()
{
	int ret;
	int fd[2];
	pid_t pid;
	char buf[10];
	struct stat statbuf;
	off_t size;
	struct sigaction act;
	int cnt;

	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGPIPE, &act, NULL);

	cnt = 0;
	ret = pipe(fd);
	if (ret < 0)
		err_sys("pipe");

	ret = fpathconf(fd[0], _PC_PIPE_BUF);
	if (ret < 0)
		err_sys("fpathconf");

	printf("pip buf size: %d\n", ret);

	fstat(fd[0], &statbuf);
	if (S_ISFIFO(statbuf.st_mode)) {
		printf("fd[0] is pipe file descriptor!\n");
	}

	size = statbuf.st_size;
	printf("size = %lld\n", (long long)size);


	if ((pid = fork()) < 0) {
		err_sys("fork");
	} else if (pid == 0) {
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);

		for (;;) {
#if 0
			ret = read(STDIN_FILENO, buf, 10);
			if (ret < 0)
				err_sys("read");
			else if (ret == 0) {
				printf("child read EOF!\n");
				break;
			}
			printf("child read from pipe[%d]: %s\n",ret, buf);

			buf[1] = 0;
			ret = atoi(buf);
			if (ret > 5) {
				printf("child close pipe!\n");
				close(STDIN_FILENO);
				break;
			}
#else
		//	ret = fread(buf, 1, 5, stdin);
			gets(buf);
			printf("gets buf: %s\n", buf);

		//	break;
#endif

			sleep(1);

		//	break;
		}
	} else {
		close(fd[0]);

		for (;;) {
			sprintf(buf, "%d\n", ++cnt);
			ret = write(fd[1], buf, 2);
			if (ret < 0) {
				err_sys("parent write pipe failed");
			}
			sleep(1);

			if (cnt >= 5) {
				printf("parent exit!\n");
				close(fd[1]);
				break;
			}
		}
	}

	exit(0);
}

int test_pipe_page()
{
#define		DEF_PAGER		 "/usr/bin/less"

	int ret;
	int fd[2];
	int pid;
	FILE *fp;
	char line[MAXLINE];
	int n;
	char *pager;
	char *argv0;

	if ((fp = fopen("pipe_page.txt", "r")) < 0)
		err_sys("fopen");

	if (pipe(fd) < 0)
		err_sys("pipe");

	if ((pid = fork()) < 0)
		err_sys("fork");
	else if (pid > 0) {
		close(fd[0]);

		while (fgets(line, MAXLINE, fp) != NULL) {
			n = strlen(line);
			if (write(fd[1], line, n) != n)
				err_sys("write");
		}
		if (ferror(fp))
			err_sys("fgets error");

		close(fd[1]);

		if (waitpid(pid, NULL, 0) < 0)
			err_sys("waitpid");
	} else {
		close(fd[1]);

		if (fd[0] != STDIN_FILENO) {
			if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO) {
				err_sys("dup2");
			}
			close(fd[0]);
		}

		if ((pager = getenv("PAGER")) == NULL) {
			pager = DEF_PAGER;
		}

		if ((argv0 = strrchr(pager, '/')) != NULL)
			argv0++;
		else
			argv0 = pager;

		if (execl(pager, argv0, (char*)0) < 0)
			err_sys("execl");
	}

	exit(0);
}

static int coprocess_add2()
{
	int a1, a2;
	char buf[MAXLINE];
	int n;

#if 0
	while (read(STDIN_FILENO, buf, MAXLINE) > 0) {
		if (sscanf(buf, "%d%d", &a1, &a2) == 2) {
			sprintf(buf, "%d\n", a1 + a2);
			n = strlen(buf);
			if (write(STDOUT_FILENO, buf, n) != n)
				err_sys("write");
		} else {
			sprintf(buf, "Invalid arguments!\n");
			n = strlen(buf);
			if (write(STDOUT_FILENO, buf, n) != n)
				err_sys("write");
		}
	}
#else

	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);

	while (fgets(buf, 100, stdin) != NULL) {
		if (sscanf(buf, "%d%d", &a1, &a2) == 2) {
			printf("%d\n", a1 + a2);
		} else {
			printf("invalid arguments!\n");
		}
	}
#endif

	exit(0);
}

static void sig_pipe(int sig)
{
	puts("SIGPIPE!");
	exit(1);
}

int test_coprocess()
{
	int ret;
	int fd1[2], fd2[2];
	pid_t pid;
	char buf[100];
	int n;

	signal(SIGPIPE, sig_pipe);

	if (pipe(fd1) < 0 || pipe(fd2) < 0)
		err_sys("pipe");

	if ((pid = fork()) < 0) {
		err_sys("fork");
	} else if (pid == 0) {
		close(fd1[1]);
		close(fd2[0]);

		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) < 0)
				err_sys("dup2");
			close(fd1[0]);
		}

		if (fd2[1] != STDOUT_FILENO) {
			if (dup2(fd2[1], STDOUT_FILENO) < 0)
				err_sys("dup2");
			close(fd2[1]);
		}

		if (execl("add2", "add2", (char*)0) < 0)
			err_sys("execl");
		exit(1);
	} else {
		close(fd1[0]);
		close(fd2[1]);

		while ((fgets(buf, 100, stdin)) != NULL) {
			n = strlen(buf);
			if ((write(fd1[1], buf, n)) != n)
				err_sys("write");
			if ((n = read(fd2[0], buf, 100)) < 0)
				err_sys("read");
			else if (n == 0) {
				err_msg("child closed pipe");
				break;
			}
			buf[n] = '\0';
		//	fputs(buf, stdout);
			printf("get result: %s", buf);
		}

		if (ferror(stdin))
			err_sys("fgets");

		exit(0);
	}

	return 0;
}

#define		MSG_QUEUE_KEY		2

#define		MSG_BUF_LEN		100

struct msg_struct {
	long type_id;
	int length;
	char buf[];
};

int test_msgqueue()
{
	int ret;
	pid_t pid;
	key_t msg_key = MSG_QUEUE_KEY;
	struct msg_struct msg, *pMsg;
	int count = 0;
	
// 	printf("sizeof struct msg_struct is %d\n", sizeof(struct msg_struct));
// 	exit(0);

	pMsg = (struct msg_struct*)malloc((sizeof(struct msg_struct) + MSG_BUF_LEN));

	msg_key = msgget(MSG_QUEUE_KEY, 0);
	if (msg_key < 0) {
		msg_key = msgget(MSG_QUEUE_KEY, IPC_CREAT | IPC_EXCL | 0666);
		if (msg_key < 0)
			err_sys("msgget");
	}

	if ((pid = fork()) < 0) {
		err_sys("fork");
	} else if (pid == 0) {
		for (;;) {
			ret = msgrcv(msg_key, pMsg, MSG_BUF_LEN, -4, IPC_NOWAIT);
			if (ret < 0) {
				if (errno == ENOMSG) {
					printf("child next...\n");
					usleep(100000);
					continue;
				}
				err_sys("child msgrcv");
			}
			*(pMsg->buf + pMsg->length) = '\0';
			printf("[child received msg] typID: %d, %s", pMsg->type_id, pMsg->buf);

			sleep(1);
		}


	} else {
		for (;;) {
			++count;
			pMsg->type_id = count;
			sprintf(pMsg->buf, "hello workd: %d\n", count);
			pMsg->length = strlen(pMsg->buf);
			ret = msgsnd(msg_key, pMsg, sizeof(struct msg_struct) - sizeof(long) + pMsg->length, IPC_NOWAIT);
			if (ret < 0) {
				err_ret("msgsnd");
			}

			sleep(1);

			if (count >= 5) {
				ret = msgctl(msg_key, IPC_RMID, NULL);
				if (ret < 0)
					err_ret("msgctl");

				printf("parent exit.\n");
				break;
			}
		}
	}

	exit(0);

	return 0;
}

#define		SEM_KEY		3
#define		SEM_TOTAL	3

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

int test_semaphore()
{
	int ret;
	pid_t pid;
	key_t sem_key;
	union semun s_um;
	struct semid_ds s_ds;

	s_um.buf = &s_ds;

	sem_key = semget(SEM_KEY, 0, 0);
	if (sem_key < 0) {
		sem_key = semget(SEM_KEY, SEM_TOTAL, IPC_CREAT|IPC_EXCL|0666);
		if (sem_key < 0)
			err_sys("semget");
	}

	ret = semctl(sem_key, 1, IPC_STAT, s_um);
	if (ret < 0)
		err_sys("semctl");



	return 0;
}
