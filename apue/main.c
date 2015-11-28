#include "includes.h"
#include <gnu/libc-version.h>


#ifdef WHR
#define  xx 1
#else
#define xx 0
#endif

int main(int argc, char *argv[])
{
	int ret;
	char version[100];

	confstr(_CS_GNU_LIBC_VERSION, version, 100);
	printf("version: %s\n", version);

	printf("Libc version: %s\n", gnu_get_libc_version());
	printf("Libc release: %s\n", gnu_get_libc_release());

	ret = pathconf("/", _PC_PATH_MAX);  //4096

	ret = pathconf("/", _POSIX_NO_TRUNC);


	read_dir();

	
	printf("xx = %d\n", xx);



	return 0;
}