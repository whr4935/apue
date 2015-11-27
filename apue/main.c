#include <stdio.h>
#include <gnu/libc-version.h>
#include <unistd.h>
#include <apue.h>

#include "includes.h"

#ifdef WHR
#define  xx 1
#else
#define xx 0
#endif

int main(int argc, char *argv[])
{
	char sz[] = "Hello, World!\n";	/* Hover mouse over "sz" while debugging to see its contents */
	printf("%s", sz);	
	fflush(stdout); /* <============== Put a breakpoint here */

	char version[100];

	confstr(_CS_GNU_LIBC_VERSION, version, 100);
	printf("version: %s\n", version);

	printf("Libc version: %s\n", gnu_get_libc_version());
	printf("Libc release: %s\n", gnu_get_libc_release());

	read_dir();

	
	printf("xx = %d\n", xx);



	return 0;
}