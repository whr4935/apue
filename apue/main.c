#include "main.h"


int main(int argc, char *argv[])
{
	int i;

	for (i = 0; i < ARRAY_SIZE(test_func_array); ++i) {
		if (test_func_array[i]) {
			test_func_array[i]();
		}
	}

	return 0;
}