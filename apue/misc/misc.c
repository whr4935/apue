#include "misc.h"
#include <includes.h>
#include <gnu/libc-version.h>

int TEST_misc(int argc, char **argv)
{

	//misc();
	//test_strtok();
	//test_exit();
	test_getopt(argc, argv);

	return 0;
}

int misc()
{
	int ret;
	int fd;

	char version[100];

	confstr(_CS_GNU_LIBC_VERSION, version, 100);
	printf("version: %s\n", version);

	printf("Libc version: %s\n", gnu_get_libc_version());
	printf("Libc release: %s\n", gnu_get_libc_release());

	ret = pathconf("/", _PC_PATH_MAX);  //4096

	ret = pathconf("/", _POSIX_NO_TRUNC);


	return 0;
}

int test_strtok()
{
	char test_string[] = "string1 string2 string3";
	char *delim = " ";
	char *p;
	char *save_ptr = NULL;

#if 1
	if ((p = strtok(test_string, delim)) == NULL) {
		return -1;
	}
	printf("%s\n", p);

	while ((p = strtok(NULL, delim)) != NULL) {
		printf("%s\n", p);
	}
#else
	if ((p = strtok_r(test_string, delim, &save_ptr)) == NULL)
		return -1;
	printf("%s\n", p);

	while ((p = strtok_r(NULL, delim, &save_ptr)) != NULL)
		printf("%s\n", p);
#endif

	return 0;
}

int test_exit()
{
	int ret;
	int prog_exit_value;
	int v;

	ret = system("./exit_with_1");
	v = WIFEXITED(ret);
	if (WIFEXITED(ret)) {
		prog_exit_value = WEXITSTATUS(ret);
	}
	printf("ret = %d\n", ret);

	return 0;
}

int test_getopt(int argc,char **argv)
{
	int i;
	int c;
	char *tok = "a:bcd:e:";

	while ((c = getopt(argc, argv, tok)) != -1) {
		switch (c) {
		case 'a':
			printf("option: %c, arg: %s, optind = %d\n", optarg, optind);
			break;

		case 'b'...'e':
			printf("option: %c\n", c);
			break;

		case '?':
			printf("unrecgonize option -%c\n", optopt);
			break;
		}
	}

	return 0;
}
