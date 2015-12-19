#include <includes.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <bits/local_lim.h>
#include <time.h>

int TEST_file_dir()
{
	test_pread();
//	read_dir();
//	test_chmod();
//	temporary_file();
//	mem_file();
//	test_pwd();
//	test_identification();
//	test_time();

	return 0;
}

int test_pread()
{
	int fd;
	int pos;
	int ret;
	char buf[100];
	int i;
	char *filename = tmpnam(NULL);

	fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0)
		err_sys("open");

	for (i = 0; i < ARRAY_SIZE(buf); ++i) {
		buf[i] = i;
	}

	ret = write(fd, buf, ARRAY_SIZE(buf));
	if (ret < 0) {
		if (errno == EINTR) {
			ret = 0;
		} else {
			err_sys("write");
		}
	}

	pos = lseek(fd, 10, SEEK_SET);
	printf("before pread, pos = %d\n", pos);

	memset(buf, 0, ARRAY_SIZE(buf));
	pread(fd, buf, 10, 10);

	pos = lseek(fd, 0, SEEK_CUR);
	printf("after pread, pos = %d\n", pos);

	close(fd);
}

int read_dir()
{
	DIR *dp;
	struct dirent *dirp;
	int fd;
	int ret;
	char buf[50];
	int dup_fd;
	FILE* fp;
	int fd_log;

	ret = fcntl(STDOUT_FILENO, F_GETFL, 0);
	if (ret < 0) {
		err_sys("fcntl");
	}
	
	switch (O_ACCMODE & ret) {
	case O_RDONLY:
		printf("read only!\n");
		break;

	case O_WRONLY:
		printf("write only!\n");
		break;

	case O_RDWR:
		printf("read and write\n");
		break;

	default:
		break;
	}

	if (ret & O_APPEND) {
		printf("open append!\n");
	}

	if (ret & O_NONBLOCK) {
		printf("no block!\n");
	}

	if (ret & O_SYNC) {
		printf("open sync!\n");
	}

	fd_log = open("../log_file.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd_log < 0) {
		err_sys("open");
	}

	ret = fcntl(fd_log, F_GETFL);
	if (ret < 0)
		err_sys("fcntl");
	
	if (ret & O_SYNC) {
		printf("log file O_SYNC is set!\n"); 
	} else {
		printf("log file O_SYNC is not set!\n");

		ret |= O_SYNC;
		ret = fcntl(fd_log, F_SETFL, ret);
		if (ret < 0)
			err_sys("fcntl");

		ret = fcntl(fd_log, F_GETFL);
		if (ret & O_SYNC) {
			printf("log file set O_SYNC success!\n");
		}
	}

	if (dup2(fd_log, STDOUT_FILENO) < 0) {
		err_sys("dup2");
	}

	printf("haha!\n");
//	close(STDOUT_FILENO);
	close(fd_log);
	return 0;


// 	dup_fd = dup(STDOUT_FILENO);
// 	if (dup_fd < 0) {
// 		err_sys("dup error");
// 	}
// 	fp = fdopen(dup_fd, "w");
// 	if (fp < 0)
// 		err_sys("fdopen\n");
// 
// 	fputs("hello dup!\n", fp);
// 	fclose(fp);


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

int test_chmod()
{
#define		TEST_FILE_NAME	"../a.txt"
	int ret;
	int mask;
	int fd;
	struct stat statbuf;
	mode_t fmode;

	mask = umask(0);
	printf("mask = %o\n", mask);

	fd = creat(TEST_FILE_NAME, S_IRWXU|S_IRWXG|S_IRWXO);
	if (fd < 0)
		err_sys("create");

	ret = fstat(fd, &statbuf);
	if (ret < 0)
	err_sys("fstat");

	fmode = statbuf.st_mode;
	printf("fmode = %o\n", fmode);

	printf("uid = %d, gid = %d\n", statbuf.st_uid, statbuf.st_gid);

	close(fd);

	ret = access(TEST_FILE_NAME, F_OK);
	if (ret < 0)
		err_sys("access");

	ret = access(TEST_FILE_NAME, R_OK|W_OK|X_OK);
	if (ret < 0)
		err_sys("access");

	printf("access test ok!\n");

	ret = chmod(TEST_FILE_NAME, fmode|S_ISUID|S_ISGID);
	if (ret < 0)
		err_sys("chmod");
	printf("chmod ok!\n");

	fmode &= ~(S_ISUID | S_ISGID);

	ret = chmod(TEST_FILE_NAME, fmode & ~S_IXUSR | S_ISUID);
	if (ret < 0)
		err_sys("chmod");

	ret = chmod(TEST_FILE_NAME, fmode | S_ISVTX);
	if (ret < 0)
		err_sys("chmod");

	fd = open(TEST_FILE_NAME, O_RDWR);
	if (fd < 0)
		err_sys("open");

	ret = lseek(fd, 1<<16, SEEK_SET);
	if (ret == -1)
		err_sys("lseek");

	printf("current file offst: %d\n", ret);

	ret = write(fd, &ret, sizeof(int));
	if (ret < 0)
		err_sys("write");
	else if (ret != sizeof(int))
		err_ret("write %d bytes!\n", ret);

	close(fd);

	ret = truncate(TEST_FILE_NAME, (1<<31)-1);
	if (ret < 0)
		err_sys("truncate");

	return 0;
}

int temporary_file()
{
	char name[L_tmpnam], line[MAXLINE];
	FILE *fp;
	int fd;
	int ret;

	printf("%s\n", tmpnam(NULL));

	tmpnam(name);

	printf("name = %s\n", name);

	fp = tmpfile();
	if (fp == NULL)
		err_sys("tmpfile");

	fputs("one line test!\n", fp);
	rewind(fp);

	if (fgets(line, MAXLINE, fp) == NULL)
		err_sys("fgets");
	
	fputs(line, stdout);
	line[0] = '\0';
	printf("end\n");

	strcpy(name, "../whr_XXXXXX.txt");
	fd = mkstemp(name);
	if (fd < 0)
		err_sys("mkstemp");

	ret = dprintf(fd, "hello, mkstemp!\n");
	close(fd);

	fd = open(name, O_RDWR);
	if (fd < 0)
		err_sys("open");

	ret = read(fd, line, MAXLINE);
	if (ret <= 0)
		err_quit("read return: %d\n", ret);

	fputs(line, stdout);
	puts("mkstemp end!");

	if (access(name, F_OK) == 0) {
		unlink(name);
	}

	ret = close(fd);

	return 0;
}

int mem_file()
{
#define BSZ 48

	FILE *fp;
	char buf[BSZ];
	int ret;

	memset(buf, 'a', BSZ - 2);
	buf[BSZ - 2] = '\0';
	buf[BSZ - 1] = 'X';
	if ((fp = fmemopen(buf, BSZ, "w+")) == NULL)
		err_sys("fmemopen failed");

	printf("initial buffer contents: %s\n", buf);
	fprintf(fp, "hello, world");
	printf("before flush: %s\n", buf);
	fflush(fp);
	printf("after fflush: %s\n", buf);
	printf("len of string in buf = %ld\n", (long)strlen(buf));
	memset(buf, 'b', BSZ - 2);
	buf[BSZ - 2] = '\0';
	buf[BSZ - 1] = 'X';
	fprintf(fp, "hello, world");
	fseek(fp, 0, SEEK_SET);
	printf("after fseek: %s\n", buf);
	printf("len of string in buf = %ld\n", (long)strlen(buf));
	memset(buf, 'c', BSZ - 2);
	buf[BSZ - 2] = '\0';
	buf[BSZ - 1] = 'X';
	fprintf(fp, "hello, world");
	fflush(fp);
	

 	ret = fseek(fp, 47, SEEK_SET);
// 	ret = fprintf(fp, "MN");
// 	ret = fflush(fp);

//	ret = fseek(fp, -1, SEEK_END);
	ret = fputc('K', fp);
	ret = fflush(fp);
	if (ret < 0) {
		err_ret("fflush");
	}
	ret = ftell(fp);
	
	fclose(fp);
	printf("after fclose: %s\n", buf);
	printf("len of string in buf = %ld\n", (long)strlen(buf));


	return 0;
}

int test_pwd()
{
	struct passwd* pwd;

	pwd = getpwuid(0);
	if (pwd == NULL)
		err_sys("getpwuid");

	pwd = getpwnam("root");

	return 0;
}

int test_identification()
{
	struct utsname name;
	char name_buf[HOST_NAME_MAX];
	int ret;

	ret = uname(&name);
	if (ret < 0)
		err_sys("uname");
	
	ret = gethostname(name_buf, HOST_NAME_MAX);
	if (ret < 0)
		err_sys("gethostname");


	return 0;
}

int test_time()
{
	struct timespec t_spec;
	int ret;
	time_t t, t2;
	struct tm *t_tm;
	char t_buf[30];
	struct tm tm_buf;
	char *pret;

	ret = clock_gettime(CLOCK_REALTIME, &t_spec);
	if (ret < 0)
		err_sys("clock_gettime");

	t = t_spec.tv_sec;
	t_tm = localtime(&t);
	ret = strftime(t_buf, 30, "%F %T", t_tm);
	if (ret == 0) {
		err_ret("strftime convert failed!\n");
	} else {
		printf("time: %s\n", t_buf);
	}

	pret = strptime(t_buf, "%Y-%m-%d %H:%M:%S", &tm_buf);
	if (pret == NULL) {
		err_msg("strptime failed!");
	} else {
		t2 = mktime(&tm_buf);
		if (t2 != t) {
			err_msg("mktime failed!");
		} else {
			printf("mktime success: %d\n", t2);
		}
	}

	return 0;
}

int set_cloexec(int fd)
{
	int ret;

	ret = fcntl(fd, F_GETFD, 0);
	if (ret < 0)
		err_sys("fcntl");
	ret |= O_CLOEXEC;
	ret = fcntl(fd, F_SETFD, ret);
	if (ret < 0)
		err_sys("fcntl");

	return 0;
}