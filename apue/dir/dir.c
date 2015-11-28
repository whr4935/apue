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