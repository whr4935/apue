#include "ipc.h"
#include <includes.h>

//////////////////////////////////////////////////////////////////////////
static int coprocess_add2();

//////////////////////////////////////////////////////////////////////////
int TEST_ipc()
{
	//test_pipe();
	//test_pipe_page();
	//coprocess_add2();
	//test_coprocess();
	//test_msgqueue();
	//test_semaphore();
	//test_shm();
	//test_posix_semaphore();
	test_network_ipc();

	return 0;
}

int test_pipe()
{
	int ret;
	int fd[2];
	pid_t pid;
	char buf[10];
	struct stat statbuf;
	off_t size;
	struct sigaction act;
	int cnt;

	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGPIPE, &act, NULL);

	cnt = 0;
	ret = pipe(fd);
	if (ret < 0)
		err_sys("pipe");

	ret = fpathconf(fd[0], _PC_PIPE_BUF);
	if (ret < 0)
		err_sys("fpathconf");

	printf("pip buf size: %d\n", ret);

	fstat(fd[0], &statbuf);
	if (S_ISFIFO(statbuf.st_mode)) {
		printf("fd[0] is pipe file descriptor!\n");
	}

	size = statbuf.st_size;
	printf("size = %lld\n", (long long)size);


	if ((pid = fork()) < 0) {
		err_sys("fork");
	} else if (pid == 0) {
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);

		for (;;) {
#if 0
			ret = read(STDIN_FILENO, buf, 10);
			if (ret < 0)
				err_sys("read");
			else if (ret == 0) {
				printf("child read EOF!\n");
				break;
			}
			printf("child read from pipe[%d]: %s\n",ret, buf);

			buf[1] = 0;
			ret = atoi(buf);
			if (ret > 5) {
				printf("child close pipe!\n");
				close(STDIN_FILENO);
				break;
			}
#else
			//	ret = fread(buf, 1, 5, stdin);
			gets(buf);
			printf("gets buf: %s\n", buf);

			//	break;
#endif

			sleep(1);

			//	break;
		}
	} else {
		close(fd[0]);

		for (;;) {
			sprintf(buf, "%d\n", ++cnt);
			ret = write(fd[1], buf, 2);
			if (ret < 0) {
				err_sys("parent write pipe failed");
			}
			sleep(1);

			if (cnt >= 5) {
				printf("parent exit!\n");
				close(fd[1]);
				break;
			}
		}
	}

	exit(0);
}

int test_pipe_page()
{
#define		DEF_PAGER		 "/usr/bin/less"

	int ret;
	int fd[2];
	int pid;
	FILE *fp;
	char line[MAXLINE];
	int n;
	char *pager;
	char *argv0;

	if ((fp = fopen("pipe_page.txt", "r")) < 0)
		err_sys("fopen");

	if (pipe(fd) < 0)
		err_sys("pipe");

	if ((pid = fork()) < 0)
		err_sys("fork");
	else if (pid > 0) {
		close(fd[0]);

		while (fgets(line, MAXLINE, fp) != NULL) {
			n = strlen(line);
			if (write(fd[1], line, n) != n)
				err_sys("write");
		}
		if (ferror(fp))
			err_sys("fgets error");

		close(fd[1]);

		if (waitpid(pid, NULL, 0) < 0)
			err_sys("waitpid");
	} else {
		close(fd[1]);

		if (fd[0] != STDIN_FILENO) {
			if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO) {
				err_sys("dup2");
			}
			close(fd[0]);
		}

		if ((pager = getenv("PAGER")) == NULL) {
			pager = DEF_PAGER;
		}

		if ((argv0 = strrchr(pager, '/')) != NULL)
			argv0++;
		else
			argv0 = pager;

		if (execl(pager, argv0, (char*)0) < 0)
			err_sys("execl");
	}

	exit(0);
}

static int coprocess_add2()
{
	int a1, a2;
	char buf[MAXLINE];
	int n;

#if 0
	while (read(STDIN_FILENO, buf, MAXLINE) > 0) {
		if (sscanf(buf, "%d%d", &a1, &a2) == 2) {
			sprintf(buf, "%d\n", a1 + a2);
			n = strlen(buf);
			if (write(STDOUT_FILENO, buf, n) != n)
				err_sys("write");
		} else {
			sprintf(buf, "Invalid arguments!\n");
			n = strlen(buf);
			if (write(STDOUT_FILENO, buf, n) != n)
				err_sys("write");
		}
	}
#else

	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);

	while (fgets(buf, 100, stdin) != NULL) {
		if (sscanf(buf, "%d%d", &a1, &a2) == 2) {
			printf("%d\n", a1 + a2);
		} else {
			printf("invalid arguments!\n");
		}
	}
#endif

	exit(0);
}

static void sig_pipe(int sig)
{
	puts("SIGPIPE!");
	exit(1);
}

int test_coprocess()
{
	int ret;
	int fd1[2], fd2[2];
	pid_t pid;
	char buf[100];
	int n;

	signal(SIGPIPE, sig_pipe);

	if (pipe(fd1) < 0 || pipe(fd2) < 0)
		err_sys("pipe");

	if ((pid = fork()) < 0) {
		err_sys("fork");
	} else if (pid == 0) {
		close(fd1[1]);
		close(fd2[0]);

		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) < 0)
				err_sys("dup2");
			close(fd1[0]);
		}

		if (fd2[1] != STDOUT_FILENO) {
			if (dup2(fd2[1], STDOUT_FILENO) < 0)
				err_sys("dup2");
			close(fd2[1]);
		}

		if (execl("add2", "add2", (char*)0) < 0)
			err_sys("execl");
		exit(1);
	} else {
		close(fd1[0]);
		close(fd2[1]);

		while ((fgets(buf, 100, stdin)) != NULL) {
			n = strlen(buf);
			if ((write(fd1[1], buf, n)) != n)
				err_sys("write");
			if ((n = read(fd2[0], buf, 100)) < 0)
				err_sys("read");
			else if (n == 0) {
				err_msg("child closed pipe");
				break;
			}
			buf[n] = '\0';
			//	fputs(buf, stdout);
			printf("get result: %s", buf);
		}

		if (ferror(stdin))
			err_sys("fgets");

		exit(0);
	}

	return 0;
}

#define		MSG_QUEUE_KEY		2

#define		MSG_BUF_LEN		100

struct msg_struct {
	long type_id;
	int length;
	char buf[];
};

int test_msgqueue()
{
	int ret;
	pid_t pid;
	key_t msg_key = MSG_QUEUE_KEY;
	struct msg_struct msg, *pMsg;
	int count = 0;

	// 	printf("sizeof struct msg_struct is %d\n", sizeof(struct msg_struct));
	// 	exit(0);

	pMsg = (struct msg_struct*)malloc((sizeof(struct msg_struct) + MSG_BUF_LEN));

	msg_key = msgget(MSG_QUEUE_KEY, 0);
	if (msg_key < 0) {
		msg_key = msgget(MSG_QUEUE_KEY, IPC_CREAT | IPC_EXCL | 0666);
		if (msg_key < 0)
			err_sys("msgget");
	}

	if ((pid = fork()) < 0) {
		err_sys("fork");
	} else if (pid == 0) {
		for (;;) {
			ret = msgrcv(msg_key, pMsg, MSG_BUF_LEN, -4, IPC_NOWAIT);
			if (ret < 0) {
				if (errno == ENOMSG) {
					printf("child next...\n");
					usleep(100000);
					continue;
				}
				err_sys("child msgrcv");
			}
			*(pMsg->buf + pMsg->length) = '\0';
			printf("[child received msg] typID: %d, %s", pMsg->type_id, pMsg->buf);

			sleep(1);
		}


	} else {
		for (;;) {
			++count;
			pMsg->type_id = count;
			sprintf(pMsg->buf, "hello workd: %d\n", count);
			pMsg->length = strlen(pMsg->buf);
			ret = msgsnd(msg_key, pMsg, sizeof(struct msg_struct) - sizeof(long) + pMsg->length, IPC_NOWAIT);
			if (ret < 0) {
				err_ret("msgsnd");
			}

			sleep(1);

			if (count >= 5) {
				ret = msgctl(msg_key, IPC_RMID, NULL);
				if (ret < 0)
					err_ret("msgctl");

				printf("parent exit.\n");
				break;
			}
		}
	}

	exit(0);

	return 0;
}

#define		SEM_KEY		3
#define		SEM_TOTAL	3

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

int test_semaphore()
{
	int ret;
	pid_t pid;
	key_t sem_key;
	int sem_id;
	union semun s_um;
	struct semid_ds s_ds;

	s_um.buf = &s_ds;

	sem_key = semget(SEM_KEY, 0, 0);
	if (sem_key < 0) {
		sem_key = semget(SEM_KEY, SEM_TOTAL, IPC_CREAT | IPC_EXCL | 0666);
		if (sem_key < 0)
			err_sys("semget");
	}

	ret = semctl(sem_key, 1, IPC_STAT, s_um);
	if (ret < 0)
		err_sys("semctl");



	return 0;
}

#define		SHM_KEY		6
#define		SHM_SIZE	10
int test_shm()
{
	int ret;
	key_t key;
	int shm_id;
	pid_t pid;
	void *addr;
	int val;

	key = SHM_KEY;

	srand(time(NULL));
	val = rand() % 1000;

	shm_id = shmget(key, 0, 0);
	if (shm_id < 0) {
		shm_id = shmget(key, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666);
		if (shm_id < 0)
			err_sys("shmget");
	}

	TELL_WAIT();

	if ((pid = fork()) < 0) {
		err_sys("fork");
	} else if (pid == 0) {
		WAIT_PARENT();
		addr = shmat(shm_id, 0, SHM_RDONLY);
		if (addr == -1) {
			goto child_exit;
		}

		val = ((int*)addr)[0];
		printf("child get value: %d\n", val);

		//	*(int*)addr = 89;

	child_exit:
		printf("child ready exit...\n");
		TELL_PARENT(getppid());
		exit(0);
	} else {
		addr = shmat(shm_id, 0, 0);
		if (addr == -1) {
			err_ret("shmat");
			ret = shmctl(shm_id, IPC_RMID, 0);
			if (ret < 0)
				err_sys("shmctl");
		}

		*((int*)addr) = val;
		printf("parent set value %d\n", val);

		ret = shmdt(addr);
		if (ret < 0)
			err_sys("shmdt");

		TELL_CHILD(pid);
		WAIT_CHILD();


		addr = shmat(shm_id, 0, SHM_RDONLY);
		if (addr == -1) {
			err_ret("shmat");
			ret = shmctl(shm_id, IPC_RMID, 0);
			if (ret < 0)
				err_sys("shmctl");
		}
		((char*)addr)[0] = 2;

		printf("parent ready exit...\n");

		ret = shmctl(shm_id, IPC_RMID, 0);
		if (ret < 0)
			err_sys("shmctl");
	}

	exit(0);

	return 0;
}

#define		POSIX_SEM_KEY	6
#define		POSIX_SEM_VALUE	0

#define		USE_NAMED_POSIX_SEM		0

int test_posix_semaphore()
{
	int ret;
	char namebuf[20];
	sem_t *p_sem;
	sem_t sem;
	pid_t pid;
	int val = 0;
	int shm_id;
	int *addr;
	struct timespec t_spec;

#if USE_NAMED_POSIX_SEM
	sprintf(namebuf, "/%d.%d", getpid(), getppid());
	p_sem = sem_open(namebuf, 0);
	if (p_sem == SEM_FAILED) {
		p_sem = sem_open(namebuf, O_CREAT|O_EXCL, 0666, POSIX_SEM_VALUE);
		if (p_sem == SEM_FAILED) {
			err_sys("sem_open");
		}
	}
	ret = sem_unlink(namebuf);
	if (ret < 0)
		err_sys("sem_unlink");
#else
	ret = sem_init(&sem, 1, POSIX_SEM_VALUE);
	if (ret < 0) {
		err_sys("sem_init");
	}
	p_sem = &sem;
#endif

	shm_id = shmget(SHM_KEY, 0, 0);
	if (shm_id < 0) {
		shm_id = shmget(SHM_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
		if (shm_id < 0)
			err_sys("shmget");
	}

	if ((pid = fork()) < 0) {
		err_sys("fork");
	} else if (pid == 0) {
		addr = shmat(shm_id, 0, 0);
		if (addr == -1) {
			err_sys("shmat");
		}

		clock_gettime(CLOCK_REALTIME, &t_spec);
		t_spec.tv_sec += 1;
		ret = sem_timedwait(p_sem, &t_spec);
		if (ret < 0)
			err_sys("child sem_wait");
		val = *addr;
		printf("child get value %d\n", val);

		*addr = 2 * val;
		printf("child set value to %d\n", *addr);

		ret = sem_getvalue(p_sem, &val);
		//	printf("sem value is %d\n", val);

		sem_post(p_sem);

#if USE_NAMED_POSIX_SEM
		ret = sem_close(p_sem);
		if (ret < 0)
			err_sys("sem_close");
#else
		ret = sem_destroy(p_sem);
		if (ret < 0)
			err_sys("sem_destroy");
#endif

		printf("child exit...\n");
		exit(1);
	} else {
		addr = shmat(shm_id, 0, 0);
		if (addr == -1) {
			err_ret("shmat");
			exit(1);
		}

		//		sem_wait(p_sem);
		usleep(100000);
		printf("parent set value 23\n");
		*addr = 23;
		sem_post(p_sem);

		printf("parent sleep...\n");
		sleep(2);

		ret = sem_wait(p_sem);
		if (ret < 0)
			err_sys("sem_wait");
		val = *addr;
		log_info("parent get value %d\n", val);
		ret = sem_post(p_sem);
		if (ret < 0)
			err_sys("sem_post");
		ret = shmdt(addr);
		if (ret < 0) {
			err_ret("shmdt");
			exit(1);
		}

#if USE_NAMED_POSIX_SEM
		ret = sem_close(p_sem);
		if (ret < 0)
			err_sys("sem_close");
#else
		ret = sem_destroy(p_sem);
		if (ret < 0)
			err_sys("sem_destroy");
#endif
	}

	exit(0);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
#define		LOCAL_HOST		"127.0.0.1"
#define		BAIDU_HOST		"www.baidu.com"
#define		ZOWIETEK_HOST	"www.zowietek.com"

static void print_hostent(struct hostent *h_ent)
{
	char **p;
	char nbuf[INET_ADDRSTRLEN];

	printf("============================\n");
	printf("host name: %s\n", h_ent->h_name);
	for (p = h_ent->h_aliases; *p; ++p) {
		printf("name alias: %s\n", *p);
	}
	printf("address type: %s\n", h_ent->h_addrtype == 2 ? "AF_INET" : (h_ent->h_addrtype == 23 ? "AF_INET6" : "unknown"));
	printf("address length: %d\n", h_ent->h_length);
	for (p = h_ent->h_addr_list; *p; ++p) {
		//printf("network address: %s\n", inet_ntoa(*(struct in_addr*)*p));
		printf("network address: %s\n", inet_ntop(AF_INET, (in_addr_t*)*p, nbuf, INET_ADDRSTRLEN));
	}
	printf("============================\n\n");
}

static void print_netent(struct netent *n_net)
{
	char **p;

	printf("============================\n");
	printf("network name: %s\n", n_net->n_name);
	for (p = n_net->n_aliases; *p; ++p) {
		printf("alternate network name: %s\n", *p);
	}
	printf("address type: %s\n", n_net->n_addrtype == 2 ? "AF_INET" : (n_net->n_addrtype == 23 ? "AF_INET6" : "unknown"));
	printf("network number: %d\n", ntohl(n_net->n_net));
	printf("============================\n\n");
}

static void print_protoent(struct protoent *p_net)
{
	char **p;

	printf("============================\n");
	printf("protocol name: %s\n", p_net->p_name);
	for (p = p_net->p_aliases; *p; ++p) {
		printf("alias protocol name: %s\n", *p);
	}
	printf("protocol number: %d\n", p_net->p_proto);
	printf("============================\n\n");
}

static void print_servent(struct servent *s_ent)
{
	char **p;

	printf("============================\n");
	printf("service name: %s\n", s_ent->s_name);
	for (p = s_ent->s_aliases; *p; ++p) {
		printf("alias service name: %s\n", *p);
	}
	printf("service port: %d\n", s_ent->s_port);
	printf("service protocol: %s\n", s_ent->s_proto);
	printf("============================\n\n");
}

static char* netfamily_str(int net_family)
{
	char *p = NULL;

	switch (net_family) {
	case AF_INET:
		p = "AF_INET";
		break;

	case AF_INET6:
		p = "AF_INET6";
		break;

	case AF_UNIX:
		p = "UNIX";
		break;

	case AF_UNSPEC:
		p = "Unspecified";
		break;

	default:
		p = "Unknown net family";
		break;
	}

	return p;
}

static char* socktype_str(int socktype)
{
	char *p = NULL;

	switch (socktype) {
	case SOCK_STREAM:
		p = "stream";
		break;

	case SOCK_DGRAM:
		p = "datagram";
		break;

	case SOCK_SEQPACKET:
		p = "seqpacket";
		break;

	case SOCK_RAW:
		p = "raw";
		break;

	default:
		p = "Unknown socktype";
	}

	return p;
}

static char* protocol_str(int proto)
{
	char *p = NULL;

	switch (proto) {
	case 0:
		p = "default protocl";
		break;

	case IPPROTO_TCP:
		p = "TCP";
		break;

	case IPPROTO_UDP:
		p = "UDP";
		break;

	case IPPROTO_RAW:
		p = "raw";
		break;

	default:
		p = "Uknown protocol";
		break;
	}

	return p;
}

static char* addrinfo_flags_str(int flags)
{
	static char buf[100];

	if (flags == 0) {
		sprintf(buf, "0");
	} else {
		if (flags & AI_PASSIVE) {
			sprintf(buf, " passive");
		}
		if (flags & AI_CANONNAME) {
			sprintf(buf, "canonname");
		}
		if (flags & AI_NUMERICHOST) {
			sprintf(buf, " numhost");
		}
		if (flags & AI_NUMERICSERV) {
			sprintf(buf, "numserv");
		}
		if (flags & AI_V4MAPPED) {
			sprintf(buf, "v4mapped");
		}
		if (flags & AI_ALL) {
			sprintf(buf, " all");
		}
	}

	return buf;
}

static void print_addrinfo(struct addrinfo* p_info)
{
	struct sockaddr_in *in_addr;

	printf("============================\n");
	printf("ai_flags: %s\n", addrinfo_flags_str(p_info->ai_flags));
	printf("ai_family: %s\n", netfamily_str(p_info->ai_family));
	printf("ai_socktype: %s\n", socktype_str(p_info->ai_socktype));
	printf("ai_protocol: %s\n", protocol_str(p_info->ai_protocol));
	printf("addr length: %d\n", p_info->ai_addrlen);
	if (p_info->ai_family == AF_INET) {
		in_addr = (struct sockaddr_in *)p_info->ai_addr;
		printf("socket addr: %s:%d\n", inet_ntoa(in_addr->sin_addr), in_addr->sin_port);
		printf("canonical host name: %s\n", p_info->ai_canonname);
		printf("============================\n\n");
	}
}

int test_network_ipc()
{
	int ret;
	in_addr_t naddr;
	struct in_addr in_naddr;
	char nbuf[INET_ADDRSTRLEN];
	char *paddr;
	struct hostent *h_ent;
	struct netent* n_net;
	struct protoent* p_ent;
	struct servent* s_ent;
	struct addrinfo n_info;
	struct addrinfo *p_info;
	struct sockaddr_in addr_in, *p_addr_in;
	char host_namebuf[50];
	char service_namebuf[50];

#if 0
	naddr = inet_addr(LOCAL_HOST);
	in_naddr.s_addr = naddr;
	paddr = inet_ntoa(in_naddr);
	printf("addr: %s(%#x)\n", paddr, (long)naddr);

	paddr = inet_ntop(AF_INET, &naddr, nbuf, INET_ADDRSTRLEN);
	if (paddr == NULL)
		err_sys("inet_ntop");
	printf("inet_ntop: %s %s\n", paddr, nbuf);

	ret = inet_pton(AF_INET, LOCAL_HOST, &naddr);
	if (ret < 0)
		err_sys("inet_pton");
	printf("naddr = %#x\n", naddr);
#endif

#if 0
	sethostent(1);
	while ((h_ent = gethostent()) != NULL) {
		print_hostent(h_ent);
	}
	endhostent();
#endif

#if 0
	h_ent = gethostbyname(BAIDU_HOST);
	if (h_ent == NULL)
		err_sys("gethostbyname");
	print_hostent(h_ent);

	h_ent = gethostbyname(ZOWIETEK_HOST);
	if (h_ent == NULL)
		err_sys("gethostbyname");
	print_hostent(h_ent);

	h_ent = gethostbyaddr(&in_naddr, sizeof(struct in_addr), AF_INET);
	if (h_ent < 0)
		err_sys("gethostbyaddr");
	print_hostent(h_ent);
#endif

#if 0
	setnetent(1);
	while ((n_net = getnetent()) != NULL) {
		print_netent(n_net);
	}
	endnetent();

#endif

#if 0
	setprotoent(1);
	while ((p_ent = getprotoent()) != NULL) {
		print_protoent(p_ent);
	}
	endprotoent();
#endif

#if 0
	setservent(1);
	while ((s_ent = getservent()) != NULL) {
		print_servent(s_ent);
	}
	endservent();
#endif

#if 0
	ret = getaddrinfo(BAIDU_HOST, "nfs", NULL, &p_info);
	if (ret != 0) {
		printf("%s\n", gai_strerror(ret));
		exit(1);
	}

	while (p_info) {
		print_addrinfo(p_info);
		p_info = p_info->ai_next;
	}
#endif

#if 0
	
#if 0
	ret = getaddrinfo(BAIDU_HOST, "nfs", NULL, &p_info);
	if (ret != 0) {
		printf("%s\n", gai_strerror(ret));
		exit(1);
	}
	p_addr_in = p_info->ai_addr;
#else
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = inet_addr(LOCAL_HOST);
	addr_in.sin_port = 264;
	p_addr_in = &addr_in;
#endif

	ret = getnameinfo(p_addr_in, sizeof(struct sockaddr_in), host_namebuf, 50, \
		service_namebuf, 50, NI_NAMEREQD | NI_NOFQDN);
	if (ret != 0) {
		printf("%s\n", gai_strerror(ret));
		exit(1);
	}
	printf("name: %s\n", host_namebuf);
	printf("service: %s\n", service_namebuf);
#endif

	printf("sockaddr size: %d\n", sizeof(struct sockaddr));
	printf("ipv4 sockaddr size: %d\n", sizeof(struct sockaddr_in));
	printf("ipv6 sockaddr size: %d\n", sizeof(struct sockaddr_in6));

	return 0;
}