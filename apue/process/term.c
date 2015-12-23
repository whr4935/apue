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

int my_pts_fork(int *ptrfdm, char *slave_name, int slave_namesz,
	const struct termios *slave_termios, const struct winsize *slave_winsize)
{
	int fdm, fds;
	char pts_name[20];
	pid_t pid;

	fdm = my_ptym_open(pts_name, 20);
	if (fdm < 0)
		err_sys("can't open master pty: %s, error %d", pts_name, fdm);

	if (slave_name != NULL) {
		strncpy(slave_name, pts_name, slave_namesz);
		slave_name[slave_namesz-1] = '\0';
	}

	if ((pid = fork()) < 0)
		err_sys("fork");
	else if (pid == 0) {
		if (setsid() < 0)
			err_sys("setsid");

		fds = my_pts_open(pts_name);
		if (fds < 0)
			err_sys("can't open slave pty");
		close(fdm);

		if (slave_termios != NULL) {
			if (tcsetattr(fds, TCSANOW, slave_termios) < 0)
				err_sys("tcsetattr");
		}

		if (slave_winsize != NULL) {
			if (ioctl(fds, TIOCSWINSZ, slave_winsize) < 0)
				err_sys("ioctl TIOCSWINSZ");
		}

		if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)
			err_sys("dup2");
		if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO)
			err_sys("dup2");
		if (dup2(fds, STDERR_FILENO) != STDOUT_FILENO)
			err_sys("dup2");
		close(fds);
		return 0;
	} else {
		*ptrfdm = fdm;
		return pid;
	}
}


//////////////////////////////////////////////////////////////////////////
#define  OPTSTR		"+d:einv"

static void pty_set_noecho(int);
static void pty_do_driver(char *);
static void pty_loop(int, int);

int my_pty(int argc, char **argv)
{
	int fdm, c, ignoreeof, interactive, noecho, verbose;
	pid_t pid;
	char *driver;
	char slave_name[20];
	struct termios orig_termios;
	struct winsize size;

	interactive = isatty(STDIN_FILENO);
	ignoreeof = 0;
	noecho = 0;
	verbose = 0;
	driver = 0;

	opterr = 0;
	while ((c = getopt(argc, argv, OPTSTR)) != EOF) {
		switch (c) {
		case 'd':
			driver = optarg;
			break;

		case 'e':
			noecho = 1;
			break;

		case 'i':
			ignoreeof = 1;
			break;

		case 'n':
			interactive = 1;
			break;

		case 'v':
			verbose = 1;
			break;

		case '?':
			err_quit("unrecongnized option: -%c", optopt);
			break;
		}
	}

	if (optind >= argc)
		err_quit("usage: pty [ -d driver -einv ] program [ arg ... ]");

	if (interactive) {
		if (tcgetattr(STDIN_FILENO, &orig_termios) < 0)
			err_sys("tcgetattr");

		if (ioctl(STDIN_FILENO, TIOCGWINSZ, &size) < 0)
			err_sys("ioctl TIOCGWINSZ");

		pid = pty_fork(&fdm, slave_name, sizeof(slave_name), &orig_termios, &size);
	} else {
		pid = pty_fork(&fdm, slave_name, sizeof(slave_name), NULL, NULL);
	}

	if (pid < 0) {
		err_sys("fork error");
	} else if (pid == 0) {
		if (noecho)
			pty_set_noecho(STDIN_FILENO);

		if (execvp(argv[optind], &argv[optind]) < 0)
			err_sys("can't execute: %s\n", argv[optind]);
	
		if (verbose) {
			fprintf(stderr, " slave name = %s\n", slave_name);
			if (driver != NULL)
				fprintf(stderr, "driver = %s\n", driver);
		}

		if (interactive && driver == NULL) {
			if (tty_raw(STDIN_FILENO) < 0)
				err_sys("tty_raw error");

			if (atexit(tty_atexit) < 0)
				err_sys("atexit");
		}

		if (driver)
			pty_do_driver(driver);

		pty_loop(fdm, ignoreeof);

		exit(0);
	}
}

static void pty_set_noecho(int fd)
{
	struct termios stermios;

	if (tcgetattr(fd, &stermios) < 0)
		err_sys("tcgetattr");

	stermios.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);

	stermios.c_oflag &= ~ONLCR;

	if (tcsetattr(fd, TCSANOW, &stermios) < 0)
		err_sys("tcsetattr");
}

static void pty_do_driver(char *driver)
{

}

static void pty_loop(int ptym, int ignoreeof)
{

}