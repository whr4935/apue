#include "includes.h"

#include <dirent.h>

int read_dir()
{
	DIR *dp;
	struct dirent *dirp;

	dp = opendir("/home/whr");
	if (dp == NULL) {
		err_sys("Can't open dir %s\n", "/home/whr");
	}

	while ((dirp = readdir(dp)) != NULL) {
		printf("%s\n", dirp->d_name);
	}

	closedir(dp);
}