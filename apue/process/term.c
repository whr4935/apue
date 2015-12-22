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

	int pt_fd;
	int pts_fd;

	pt_fd = posix_openpt(O_RDWR);
	if (pt_fd < 0)
		err_sys("posix_openpt");

//	grantpt(pt_fd);
	unlockpt(pt_fd);		//关键是调用unlockpt函数以后，打开从设备才会成功
	

	printf("pt name: %s\n", ptsname(pt_fd));

	pts_fd = open(ptsname(pt_fd), O_RDWR);
	if (pts_fd < 0)
		err_sys("open");

	exit(0);
}

int my_ptym_open(char *pts_name, int pts_namesz)
{
	char *ptr;
	int fdm, err;

	if ((fdm = posix_openpt(O_RDWR)) < 0)
		return -1;

	if (grantpt(fdm) < 0)
		goto errout;

	if (unlockpt(fdm) < 0)
		goto errout;

	if ((ptr = ptsname(fdm)) == NULL)
		goto errout;

	strncpy(pts_name, ptr, pts_namesz);
	pts_name[pts_namesz - 1] = '\0';
	return fdm;

errout:
	err = errno;
	close(fdm);
	errno = err;
	return -1;
}

int my_pts_open(char *pts_name)
{
	int fds;

	if ((fds = open(pts_name, O_RDWR)) < 0)
		return -1;

	return fds;
}
