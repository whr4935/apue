#include "misc.h"
#include <includes.h>
#include <gnu/libc-version.h>

int TEST_misc(int argc, char **argv)
{
	return 0;
}

int misc()
{
	int ret;

	char version[100];

	confstr(_CS_GNU_LIBC_VERSION, version, 100);
	printf("version: %s\n", version);

	printf("Libc version: %s\n", gnu_get_libc_version());
	printf("Libc release: %s\n", gnu_get_libc_release());

	ret = pathconf("/", _PC_PATH_MAX);  //4096

	ret = pathconf("/", _POSIX_NO_TRUNC);

	return 0;
}
