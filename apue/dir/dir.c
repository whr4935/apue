#include "includes.h"
#include <dirent.h>
#include <fcntl.h>

int read_dir()
{
	DIR *dp;
	struct dirent *dirp;
	int fd;
	int ret;
	char buf[50];
	int dup_fd;
	FILE* fp;
	int fd_log;

	ret = fcntl(STDOUT_FILENO, F_GETFL, 0);
	if (ret < 0) {
		err_sys("fcntl");
	}
	
	switch (O_ACCMODE & ret) {
	case O_RDONLY:
		printf("read only!\n");
		break;

	case O_WRONLY:
		printf("write only!\n");
		break;

	case O_RDWR:
		printf("read and write\n");
		break;

	default:
		break;
	}

	if (ret & O_APPEND) {
		printf("open append!\n");
	}

	if (ret & O_NONBLOCK) {
		printf("no block!\n");
	}

	if (ret & O_SYNC) {
		printf("open sync!\n");
	}

	fd_log = open("../log_file.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd_log < 0) {
		err_sys("open");
	}

	ret = fcntl(fd_log, F_GETFL);
	if (ret < 0)
		err_sys("fcntl");
	
	if (ret & O_SYNC) {
		printf("log file O_SYNC is set!\n");
	} else {
		printf("log file O_SYNC is not set!\n");

		ret |= O_SYNC;
		ret = fcntl(fd_log, F_SETFL, ret);
		if (ret < 0)
			err_sys("fcntl");

		ret = fcntl(fd_log, F_GETFL);
		if (ret & O_SYNC) {
			printf("log file set O_SYNC success!\n");
		}
	}

	if (dup2(fd_log, STDOUT_FILENO) < 0) {
		err_sys("dup2");
	}

	printf("haha!\n");
//	close(STDOUT_FILENO);
	close(fd_log);
	return 0;


// 	dup_fd = dup(STDOUT_FILENO);
// 	if (dup_fd < 0) {
// 		err_sys("dup error");
// 	}
// 	fp = fdopen(dup_fd, "w");
// 	if (fp < 0)
// 		err_sys("fdopen\n");
// 
// 	fputs("hello dup!\n", fp);
// 	fclose(fp);


	getcwd(buf, 50);
	printf("current work directory: %s\n", buf);


	dp = opendir("/home/whr");
	if (dp == NULL) {
		err_sys("Can't open dir %s\n", "/home/whr");
	}

	fd  = dirfd(dp);

	while ((dirp = readdir(dp)) != NULL) {
		printf("%s\n", dirp->d_name);

		if (dirp->d_name[0]=='.')
			continue;

		ret = openat(fd, dirp->d_name, O_RDONLY);
		if (ret < 0) {
			printf("openat %s failed!\n", dirp->d_name);
		} else {
			printf("openat %s success!\n", dirp->d_name);
			close(ret);
			break;
		}
	}

	closedir(dp);
}