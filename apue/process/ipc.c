#include "ipc.h"
#include <includes.h>

int TEST_ipc()
{
	test_pipe();

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