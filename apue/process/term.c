#include "term.h"
#include <includes.h>

int TEST_term()
{
	test_term_misc();

	return 0;
}

int test_term_misc()
{
	struct termios term;
	long vdisable;

	if (isatty(STDIN_FILENO) == 0)
		err_quit("standard input is not a terminal device");

	if ((vdisable = fpathconf(STDIN_FILENO, _PC_VDISABLE)) < 0)
		err_quit("fpathconf error or _POSIX_VDIABLE not in effect");

// 	term.c_cc[VINTR] = vdisable;
// 	term.c_cc[VEOF] = 2;

	switch (term.c_cflag & CSIZE) {
	case CS5:
		printf("5 bits/byte\n");
		break;

	case CS6:
		printf("6 bits/byte\n");
		break;

	case CS7:
		printf("7 bits/byte\n");
		break;

	case CS8:
		printf("8 bits/byte\n");
		break;

	default:
		printf("unknown bit/byte\n");
		break;
	}

// 	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) < 0)
// 		err_sys("tcsetattr");

	struct winsize w_size;
	int ret;

	ret = ioctl(STDIN_FILENO, TIOCGWINSZ, &w_size);
	if (ret < 0)
		err_sys("ioctl");
	printf("terminal size: %d X %d\n", w_size.ws_row, w_size.ws_col);

	exit(0);
}

