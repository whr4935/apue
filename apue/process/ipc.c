#include "ipc.h"
#include <includes.h>

//////////////////////////////////////////////////////////////////////////
static int coprocess_add2();

//////////////////////////////////////////////////////////////////////////
int TEST_ipc(int argc, char **argv)
{
	//test_pipe();
	//test_pipe_page();
	//coprocess_add2();
	//test_coprocess();
	//test_msgqueue();
	//test_semaphore();
	//test_shm();
	//test_posix_semaphore();
	//test_network_ipc();

	//test_ruptime_server();
	//test_ruptime_client();

	//test_uptime_server_udp();
	//test_uptime_client_udp();

	//test_unix_domain_socket();
	//test_unix_domain_socket_helper(argc, argv);

	//test_unix_socket_bind();

	//test_open_server_v1();
	//test_open_server_client_v1();

	//test_open_server_client_v2();
	test_open_server_v2(argc, argv);

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
#define		POSIX_SHM_SEM_KEY	7
#define		POSIX_SEM_VALUE	0

#define		USE_NAMED_POSIX_SEM		0

int test_posix_semaphore()
{
	int ret;
	char namebuf[20];
	sem_t *p_sem;
	pid_t pid;
	int val = 0;
	int shm_id;
	int *addr;
	struct timespec t_spec;
	int shm_sem;
	sem_t *p_shm_sem;

#if USE_NAMED_POSIX_SEM
	sprintf(namebuf, "/%d.%d", getpid(), getppid());
	p_sem = sem_open(namebuf, 0);
	if (p_sem == SEM_FAILED) {
		p_sem = sem_open(namebuf, O_CREAT|O_EXCL, 0666, POSIX_SEM_VALUE);
		if (p_sem == SEM_FAILED) {
			err_sys("sem_open");
		}
	}
	ret = sem_unlink(namebuf);		//删除命令信号量
	if (ret < 0)
		err_sys("sem_unlink");
#else
	shm_sem = shmget(POSIX_SHM_SEM_KEY, 0, 0);
	if (shm_sem < 0) {
		shm_sem = shmget(POSIX_SHM_SEM_KEY, sizeof(sem_t), IPC_CREAT | IPC_EXCL | 0600);
		if (shm_sem < 0) {
			err_sys("shmget");
		}
	}
	p_shm_sem = shmat(shm_sem, 0, 0);
	if (p_shm_sem == -1) {
		err_sys("shmat");
	}

	ret = sem_init(p_shm_sem, 1, POSIX_SEM_VALUE);		//匿名信号量需要创建在共享内存区才能跨进程
	if (ret < 0) {
		err_sys("sem_init");
	}
	p_sem = p_shm_sem;
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
		t_spec.tv_sec += 3;
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

		ret = sem_getvalue(p_sem, &val);
		printf("[sem] value is %d\n", val);

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

		ret = sem_getvalue(p_sem, &val);
		//	printf("[sem] value is %d\n", val);

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

		ret = shmctl(shm_id, IPC_RMID, 0);
		if (ret < 0)
			err_sys("shmctl");

#if USE_NAMED_POSIX_SEM
		ret = sem_close(p_sem);
		if (ret < 0)
			err_sys("sem_close");
#else
		ret = sem_destroy(p_sem);
		if (ret < 0)
			err_sys("sem_destroy");

		ret = shmdt(p_sem);
		if (ret < 0)
			err_sys("shmdt");

		ret = shmctl(shm_sem, IPC_RMID, 0);
		if (ret < 0)
			err_sys("shmctl");
#endif
	}

	exit(0);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
#define		LOCAL_HOST		"127.0.0.1"
#define		BAIDU_HOST		"www.baidu.com"
#define		ZOWIETEK_HOST	"www.zowietek.com"

static void print_sockaddr_in(struct sockaddr_in *in_addr)
{
	printf("========= SOCKADDR_IN ===========\n");
	printf("address family: %s\n", in_addr->sin_family == 2 ? "AF_INET" : (in_addr->sin_family == 23 ? "AF_INET6" : "unknown"));
	printf("address: %s\n", inet_ntoa(in_addr->sin_addr));
	printf("port: %d\n", ntohs(in_addr->sin_port));
	printf("=================================\n");
}

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
	printf("service port: %d\n", ntohs(s_ent->s_port));
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
		printf("socket addr: %s:%d\n", inet_ntoa(in_addr->sin_addr), ntohs(in_addr->sin_port));
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

#if 1
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

	in_naddr.s_addr = inet_addr(LOCAL_HOST);
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
	ret = getaddrinfo(BAIDU_HOST, "ssh", NULL, &p_info);
	if (ret != 0) {
		printf("%s\n", gai_strerror(ret));
		exit(1);
	}
	p_addr_in = p_info->ai_addr;
#else
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = inet_addr(LOCAL_HOST);
	addr_in.sin_port = 22;
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

#if 0
	printf("sockaddr size: %d\n", sizeof(struct sockaddr));
	printf("ipv4 sockaddr size: %d\n", sizeof(struct sockaddr_in));
	printf("ipv6 sockaddr size: %d\n", sizeof(struct sockaddr_in6));
#endif

	return 0;
}

//////////////////////////////////////////////////////////////////////////
#define		DAEMONIZE_RUPTIME_SERVER		0

#if DAEMONIZE_RUPTIME_SERVER
#define		server_log(arg, fmt, ...)	syslog(arg, fmt, ##__VA_ARGS__)
#else
#define		server_log(arg, fmt, ...)	printf(fmt"\n", ##__VA_ARGS__)
#endif

//////////////////////////////////////////////////////////////////////////
#define		MAXSLEEP 4
int connect_retry(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t alen)
{
	int numsec, fd;

	for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1) {
		if ((fd = socket(domain, type, protocol)) < 0)
			return -1;
		if (connect(fd, addr, alen) == 0)
			return fd;

		close(fd);
		if (numsec <= MAXSLEEP / 2)
			sleep(numsec);
	}

	return -1;
}

int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
	int fd;
	int err = 0;
	int reuse = 1;

	if ((fd = socket(addr->sa_family, type, 0)) < 0)
		return -1;

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
		goto errout;

	if (bind(fd, addr, alen) < 0) {
		server_log(LOG_ERR, "bind");
		goto errout;
	}
	if (type == SOCK_STREAM || type == SOCK_SEQPACKET) {
		if (listen(fd, qlen) < 0)
			goto errout;
	}
	return fd;

errout:
	err = errno;
	close(fd);
	errno = err;
	return -1;
}

#ifndef HOST_NAME_MAX
#define		HOST_NAME_MAX		256
#endif

#define		BUFLEN				128
#define		QLEN				10
#define		MAXADDRLEN			256

#define		POPEN_UPTIME	0

static void serve(int sockfd)
{
	int clfd;
	FILE *fp;
	pid_t pid;
	int status;
	char buf[BUFLEN];
	struct sockaddr_in in_addr;
	socklen_t slen = sizeof(struct sockaddr_in);

	if (getsockname(sockfd, &in_addr, &slen) < 0) {
		server_log(LOG_ERR, "getsockname");
		exit(1);
	}
	printf("server bound address:\n");
	print_sockaddr_in(&in_addr);

	my_set_cloexec(sockfd);
	for (;;) {
		if ((clfd = accept(sockfd, NULL, NULL)) < 0) {
			server_log(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));
			exit(1);
		}
		my_set_cloexec(clfd);

		//get client information
		if (getpeername(clfd, (struct sockaddr*)&in_addr, &slen) < 0) {
			server_log(LOG_ERR, "getpeername");
		}
		printf("\nclient information:\n");
		print_sockaddr_in(&in_addr);

#if POPEN_UPTIME
		if ((fp = popen("/usr/bin/uptime", "r")) == NULL) {
			sprintf(buf, "error: %s\n", strerror(errno));
			send(clfd, buf, strlen(buf), 0);
		} else {
			while (fgets(buf, BUFLEN, fp) != NULL)
				send(clfd, buf, strlen(buf), 0);
			pclose(fp);
		}
		close(clfd);
#else
		if ((pid = fork()) < 0) {
			server_log(LOG_ERR, "fork");
		} else if (pid == 0) {
			if (dup2(clfd, STDOUT_FILENO) != STDOUT_FILENO ||
				dup2(clfd, STDERR_FILENO) != STDERR_FILENO) {
				server_log(LOG_ERR, "dup2");
				exit(1);
			}

			execl("/usr/bin/uptime", "uptime", (char*)0);
			server_log(LOG_ERR, "execl");
			exit(1);
		} else {
			close(clfd);
			waitpid(pid, &status, 0);
		}
#endif
	}
}

static void sig_int(int signo)
{
	server_log(LOG_INFO, "ruptimed received SIGINT, exit...");

	exit(1);
}

int test_ruptime_server()
{
	struct addrinfo *alist, *aip;
	struct addrinfo hint;
	int sockfd, err, n;
	char *host;
	struct sockaddr_in *in_addr;
	struct sigaction sia;

	if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
		n = HOST_NAME_MAX;
	if ((host = malloc(n)) == NULL)
		err_sys("malloc");
	if (gethostname(host, n) < 0)
		err_sys("gethostname");

#if DAEMONIZE_RUPTIME_SERVER
	my_daemonize("ruptimed");
#endif

	sia.sa_flags = 0;
	sigemptyset(&sia.sa_mask);
	sia.sa_handler = sig_int;
	if (sigaction(SIGINT, &sia, NULL) < 0) {
		syslog(LOG_ERR, "sigaction failed!");
	}

	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((err = getaddrinfo(host, "ruptime", &hint, &alist) != 0)) {		//手动在/etc/services文件里添加 ruptime服务项
		server_log(LOG_ERR, "ruptimed: getaddrinfo: %s", gai_strerror(err));
		exit(1);
	}
	for (aip = alist; aip != NULL; aip = aip->ai_next) {
		//	print_addrinfo(aip);

		// 		in_addr = (struct sockaddr_in*)aip->ai_addr;
		// 		in_addr->sin_addr.s_addr = INADDR_ANY;

		if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0) {
			serve(sockfd);
			server_log(LOG_ERR, "exit");
			exit(0);
		}
	}
	server_log(LOG_ERR, "exit");
	exit(1);

	return 0;
}

//client
static void print_uptime(int sockfd)
{
	int n;
	char buf[BUFLEN];

	while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0) {
		write(STDOUT_FILENO, buf, n);
		if (n < 0)
			err_sys("recv");
	}
}

int test_ruptime_client()
{
	struct addrinfo* alist, *aip;
	struct addrinfo hint;
	int sockfd, err;
	char server_addr[HOST_NAME_MAX];

	if (gethostname(server_addr, HOST_NAME_MAX) < 0)
		err_sys("gethostname");
	printf("hostname: %s\n", server_addr);

	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((err = getaddrinfo(server_addr, "ruptime", &hint, &alist)) < 0) {
		err_quit("getaddrinfo: %s\n", aio_error(err));
	}

	//	print_addrinfo(alist);

	for (aip = alist; aip != NULL; aip = aip->ai_next) {
		if ((sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0, aip->ai_addr, \
			aip->ai_addrlen)) < 0) {
			err = errno;
		} else {
			print_uptime(sockfd);
			exit(0);
		}
	}

	err_exit(err, "can't connect to %s", server_addr);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// UDP uptime server
void serve_udp(int sockfd)
{
	int n;
	socklen_t alen;
	FILE *fp;
	char buf[BUFLEN];
	char abuf[MAXADDRLEN];
	struct sockaddr *addr = (struct sockaddr *)abuf;

	my_set_cloexec(sockfd);
	for (;;) {
		alen = MAXADDRLEN;
		if ((n = recvfrom(sockfd, buf, BUFLEN, 0, addr, &alen)) < 0) {
			server_log(LOG_ERR, "recvfrom");
			exit(1);
		}

		if ((fp = popen("/usr/bin/uptime", "r")) == NULL) {
			sprintf(buf, "error: %s\n", strerror(errno));
			sendto(sockfd, buf, strlen(buf), 0, addr, alen);
		} else {
			if (fgets(buf, BUFLEN, fp) != NULL) {
				sendto(sockfd, buf, strlen(buf), 0, addr, alen);
			}
			pclose(fp);
		}
	}
}

int test_uptime_server_udp()
{
	int sockfd;
	char *host;
	int n;
	struct addrinfo *alist, *aip, hint;
	int err;
	struct sockaddr_in in_addr;
	socklen_t slen;

	if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
		n = HOST_NAME_MAX;
	host = malloc(n);
	if (host == NULL) {
		server_log(LOG_ERR, "malloc");
		exit(1);
	}

	if (gethostname(host, n) < 0) {
		server_log(LOG_ERR, "gethostname");
		exit(1);
	}

	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = SOCK_DGRAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((err = getaddrinfo(host, "ruptime", &hint, &alist)) != 0) {
		server_log(LOG_ERR, "getaddrinfo: %s\n", gai_strerror(err));
		exit(1);
	}

	for (aip = alist; aip != NULL; aip = aip->ai_next) {
		if ((sockfd = socket(aip->ai_family, SOCK_DGRAM, 0)) < 0) {
			server_log(LOG_ERR, "socket");
			exit(1);
		}

		if ((err = bind(sockfd, aip->ai_addr, aip->ai_addrlen)) < 0) {
			server_log(LOG_ERR, "bind");
			exit(1);
		}

		if (getsockname(sockfd, &in_addr, &slen) < 0) {
			server_log(LOG_ERR, "getsockname");
			exit(1);
		}
		print_sockaddr_in(&in_addr);

		serve_udp(sockfd);

		server_log(LOG_ERR, "exit");
		exit(0);
	}

	server_log(LOG_ERR, "exit");
	exit(1);

	return 0;
}

#define		TIMEOUT		20
static void sigalrm(int signo)
{

}

static void print_uptime_udp(int sockfd, struct addrinfo *aip)
{
	int n;
	char buf[BUFLEN];

	buf[0] = 0;
	if (sendto(sockfd, buf, 1, 0, aip->ai_addr, aip->ai_addrlen) < 0) {
		server_log(LOG_ERR, "sendto");
		exit(1);
	}
	alarm(TIMEOUT);
	if ((n = recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL)) < 0) {
		if (errno != EINTR) {
			alarm(0);
		}
		server_log(LOG_ERR, "recvfrom: %s\n", strerror(errno));
		exit(1);
	}
	alarm(0);
	write(STDOUT_FILENO, buf, n);
}

//UDP uptime client
int test_uptime_client_udp()
{
	struct addrinfo *alist, *aip, hint;
	int err, sockfd;
	struct sigaction sa;
	char host[HOST_NAME_MAX];
	struct servent *ent;

	sa.sa_handler = sigalrm;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGALRM, &sa, NULL) < 0) {
		server_log(LOG_ERR, "sigaction");
		exit(1);
	}

	if (gethostname(host, HOST_NAME_MAX) < 0) {
		server_log(LOG_ERR, "gethostname");
		exit(1);
	}

	if ((ent = getservbyname("ruptime", "udp")) != NULL) {
		printf("port = %d\n", ntohs(ent->s_port));
		print_servent(ent);
	}

	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_DGRAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_addrlen = NULL;
	if ((err = getaddrinfo(host, "ruptime", &hint, &alist)) != 0) {
		server_log(LOG_ERR, "getaddrinfo: %s", gai_strerror(err));
		exit(1);
	}

	for (aip = alist; aip != NULL; aip = aip->ai_next) {
		if ((sockfd = socket(aip->ai_family, SOCK_DGRAM, 0)) < 0) {
			err = errno;
		} else {
			print_sockaddr_in((struct sockaddr_in*)aip->ai_addr);

			print_uptime_udp(sockfd, aip);
			exit(0);
		}
	}

	fprintf(stderr, "can't contact %s: %s\n", host, strerror(errno));
	exit(1);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
#define		NQ		3
#define		MAXMSZ	512
#define		KEY		0x123

struct threadinfo {
	int qid;
	int fd;
};

struct mymesg {
	long mtype;
	char mtext[MAXMSZ];
};

static void *helper(void *arg)
{
	int n;
	struct mymesg m;
	struct threadinfo *tip = arg;

	for (;;) {
		memset(&m, 0, sizeof(m));
		if ((n = msgrcv(tip->qid, &m, MAXMSZ, 0, MSG_NOERROR)) < 0)
			err_sys("msgrcv error");
		if (write(tip->fd, m.mtext, n) < 0)
			err_sys("write");
	}

	return (void*)0;
}



static void sig_term_handler(int signo)
{
	int qid[NQ];
	int i;

	if (signo != SIGTERM) {
		printf("wrong signal: %s\n", strsignal(signo));
		exit(1);
	}

	for (i = 0; i < NQ; ++i) {
		if ((qid[i] = msgget(KEY + i, 0)) < 0)
			continue;

		if (msgctl(qid[i], IPC_RMID, NULL) < 0)
			err_ret("msgctl");
	}

	printf("exit frome SIGTERM signal handler\n");
}

int test_unix_domain_socket()
{
	int i, n, err;
	int fd[2];
	int qid[NQ];
	struct pollfd pfd[NQ];
	struct threadinfo ti[NQ];
	pthread_t tid[NQ];
	char buf[MAXMSZ];
	struct sigaction sa;

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sig_term_handler;
	if (sigaction(SIGTERM, &sa, NULL) < 0)
		err_sys("sigaction");

	for (i = 0; i < NQ; ++i) {
		if ((qid[i] = msgget(KEY + i, 0)) < 0) {
			if ((qid[i] = msgget(KEY + i, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
				if (errno == EEXIST) {
					if ((qid[i] = msgget(KEY + i, 0)) < 0) {
						err_sys("msgget");
					} else {
						err_sys("msgget");
					}
				}
			}
		}

		printf("queue ID %d is %d\n", i, qid[i]);

		if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0)
			err_sys("socketpair error");

		pfd[i].fd = fd[0];
		pfd[i].events = POLLIN;
		ti[i].qid = qid[i];
		ti[i].fd = fd[1];
		if ((err = pthread_create(tid + i, NULL, helper, ti + i)) != 0)
			err_exit(err, "pthread_create errr");
	}

	for (;;) {
		if (poll(pfd, NQ, -1) < 0)
			err_sys("poll");

		for (i = 0; i < NQ; ++i) {
			if (pfd[i].revents & POLLIN) {
				if ((n = read(pfd[i].fd, buf, sizeof(buf))) < 0)
					err_sys("read");
				buf[n] = 0;
				printf("queue id %d, message: %s\n", i, buf);
			}
		}
	}

	exit(0);
	return 0;
}

int test_unix_domain_socket_helper(int argc, char **argv)
{
	key_t key;
	long qid;
	size_t nbytes;
	struct mymesg m;

	if (argc != 3) {
		fprintf(stderr, "usage: sendmsg KEY messages\n");
		exit(1);
	}

	key = strtol(argv[1], NULL, 0);
	if ((qid = msgget(key, 0)) < 0)
		err_sys("msgget");
	memset(&m, 0, sizeof(m));
	strncpy(m.mtext, argv[2], MAXMSZ - 1);
	nbytes = strlen(m.mtext);
	m.mtype = 1;
	if (msgsnd(qid, &m, nbytes, 0) < 0)
		err_sys("msgsnd");
	exit(0);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
int test_unix_socket_bind()
{
	struct sockaddr_un un;
	int fd, size;

	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, "foo.socket");

	if ((fd = socket(un.sun_family, SOCK_STREAM, 0)) < 0)
		err_sys("socket");

	size = offsetof(struct sockaddr_un, sun_path);
	size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	size = sizeof(struct sockaddr_un);
	if (bind(fd, &un, size) < 0) {
		if (errno == EADDRINUSE) {
			printf("warning: %s\n", strerror(errno));
			printf("rm %s\n", un.sun_path);
			if (unlink(un.sun_path) < 0)
				err_sys("unlink");

			if (bind(fd, &un, size) < 0) {
				err_sys("bind");
			}
		} else {
			err_sys("bind");
		}
	}

	printf("UNIX domain socket bound!\n");

	if (unlink(un.sun_path) < 0)
		err_sys("unlink");

	exit(0);
}

//////////////////////////////////////////////////////////////////////////
int my_serv_listen(const char *name)
{
	int fd, len, err, rval;
	struct sockaddr_un un;

	if (strlen(name) >= sizeof(un.sun_path)) {
		errno = ENAMETOOLONG;
		return -1;
	}

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		return -2;

	unlink(name);

	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(name);

	if (bind(fd, &un, len) < 0) {
		rval = -3;
		goto errout;
	}

	if (listen(fd, QLEN) < 0) {
		rval = -4;
		goto errout;
	}
	return fd;

errout:
	err = errno;
	close(fd);
	errno = err;
	return rval;
}

#define		STALE		30
int my_serv_accept(int listenfd, uid_t *uidptr)
{
	int clifd, err, rval;
	socklen_t len;
	time_t staletime;
	struct sockaddr_un un;
	struct stat statbuf;
	char *name;

	if ((name = malloc(sizeof(un.sun_path + 1))) == NULL)
		return -1;

	len = sizeof(un);
	if ((clifd = accept(listenfd, (struct sockaddr*)&un, &len)) < 0) {
		free(name);
		return -2;
	}

	len -= offsetof(struct sockaddr_un, sun_path);
	memcpy(name, un.sun_path, len);
	name[len] = 0;
	if (stat(name, &statbuf) < 0) {
		rval = -3;
		goto errout;
	}

#ifdef S_ISSOCK
	if (S_ISSOCK(statbuf.st_mode) == 0) {
		rval = -4;
		goto errout;
	}
#endif

	if ((statbuf.st_mode & (S_IRWXG | S_IRWXO)) ||
		(statbuf.st_mode & S_IRWXU) != S_IRWXU) {
		rval = -5;
		goto errout;
	}

	staletime = time(NULL) - STALE;
	if (statbuf.st_atime < staletime ||
		statbuf.st_ctime < staletime ||
		statbuf.st_mtime < staletime) {
		rval = -6;
		goto errout;
	}

	if (uidptr != NULL) {
		*uidptr = statbuf.st_uid;
	}
	unlink(name);
	free(name);
	return clifd;

errout:
	err = errno;
	close(clifd);
	free(name);
	errno = err;
	return rval;
}

#define		CLI_PATH	"/var/tmp/"
#define		CLI_PERM	S_IRWXU

int my_cli_conn(const char *name)
{
	int fd, len, err, rval;
	struct sockaddr_un un, sun;
	int do_unlink = 0;

	if (strlen(name) >= sizeof(un.sun_path)) {
		errno = ENAMETOOLONG;
		return -1;
	}

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		return -1;
	}

	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	sprintf(un.sun_path, "%s%05ld", CLI_PATH, (long)getpid());
	len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

	unlink(un.sun_path);
	if (bind(fd, (struct sockaddr*)&un, len) < 0) {
		rval = -2;
		goto errout;
	}

	if (chmod(un.sun_path, CLI_PERM) < 0) {
		rval = -3;
		do_unlink = 1;
		goto errout;
	}

	memset(&sun, 0, sizeof(sun));
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
	if (connect(fd, (struct sockaddr*)&sun, len) < 0) {
		rval = -4;
		do_unlink = 1;
		goto errout;
	}
	return fd;

errout:
	err = errno;
	close(fd);
	if (do_unlink)
		unlink(un.sun_path);
	errno = err;
	return rval;
}

int my_send_err(int fd, int errcode, const char* errmsg)
{
	int n;

	if ((n = strlen(errmsg)) > 0) {
		if (writen(fd, errmsg, n) != n)		//不能写'\0'，否则与协议冲突
			return -1;
	}

	if (errcode >= 0)
		errcode = -1;

	if (my_send_fd(fd, errcode) < 0)
		return -1;

	return 0;
}

#define CONTROLLEN	CMSG_LEN(sizeof(int))
static struct cmsghdr *cmptr = NULL;

int my_send_fd(int fd, int fd_to_send)
{
	struct iovec iov[1];
	struct msghdr msg;
	char buf[2];

	iov[0].iov_base = buf;
	iov[0].iov_len = 2;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;

	if (fd_to_send < 0) {
		msg.msg_control = NULL;
		msg.msg_controllen = 0;
		buf[1] = -fd_to_send;
		if (buf[1] == 0) {
			buf[1] = 1;
		}
	} else {
		if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL) {
			return -1;
		}
		cmptr->cmsg_level = SOL_SOCKET;
		cmptr->cmsg_type = SCM_RIGHTS;
		cmptr->cmsg_len = CONTROLLEN;
		msg.msg_control = cmptr;
		msg.msg_controllen = CONTROLLEN;
		*(int*)CMSG_DATA(cmptr) = fd_to_send;
		buf[1] = 0;
	}
	buf[0] = 0;
	if (sendmsg(fd, &msg, 0) != 2) {
		return -1;
	}

	return 0;
}

static struct cmsghdr *recv_cmptr = NULL;

int my_recv_fd(int fd, ssize_t(*userfunc)(int, const void*, size_t))
{
	int newfd, nr, status;
	char *ptr;
	char buf[MAXLINE];
	struct iovec iov[1];
	struct msghdr msg;

	status = -1;
	for (;;) {
		iov[0].iov_base = buf;
		iov[0].iov_len = sizeof(buf);
		msg.msg_iov = iov;
		msg.msg_iovlen = 1;
		msg.msg_name = NULL;
		msg.msg_namelen = 0;
		if (recv_cmptr == NULL && (recv_cmptr = malloc(CONTROLLEN)) == NULL) {
			return -1;
		}
		msg.msg_control = recv_cmptr;
		msg.msg_controllen = CONTROLLEN;
		if ((nr = recvmsg(fd, &msg, 0)) < 0) {
			err_ret("recvmsg");
			return -1;
		} else if (nr == 0) {
			err_ret("connction closed by server");
			return -1;
		}

		for (ptr = buf; ptr < &buf[nr];) {
			if (*ptr++ == 0) {
				if (ptr != &buf[nr - 1])
					err_dump("message format error");
				status = *ptr & 0xFF;
				if (status == 0) {
					if (msg.msg_controllen != CONTROLLEN)
						err_dump("status = 0 but no fd");
					newfd = *(int*)CMSG_DATA(recv_cmptr);
				} else {
					newfd = -status;
				}
				nr -= 2;
			}
		}
		if (nr > 0 && (*userfunc)(STDOUT_FILENO, buf, nr) != nr)
			return -1;

		if (status >= 0)
			return newfd;
	}
}

//////////////////////////////////////////////////////////////////////////
#define		BUFFSIZE		8192
#define		CL_OPEN			"open"

static int csopen(char *, int);

/////////////////////////////////
int test_open_server_client_v1()
{
	int n, fd;
	char buf[BUFFSIZE];
	char line[MAXLINE];

	while (fgets(line, MAXLINE, stdin) != NULL) {
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;

		if ((fd = csopen(line, O_RDONLY)) < 0) {
			printf("csopen failed!\n");
			continue;
		}

		while ((n = read(fd, buf, BUFFSIZE)) > 0) {
			if (write(STDOUT_FILENO, buf, n) != n)
				err_sys("write");
		}

		if (n < 0)
			err_sys("read");
		close(fd);
	}

	exit(0);
}

static int csopen(char *name, int oflag)
{
	pid_t	pid;
	int		len;
	char	buf[10];
	struct iovec iov[3];
	static int fd[2] = { -1, -1 };

	if (fd[0] < 0) {
		if (fd_pipe(fd) < 0) {		//fd_pipe: full_duplex pipe(a UNIX domain socket)
			err_ret("fd_pipe");
			return -1;
		}
		if ((pid = fork()) < 0) {
			err_ret("fork");
			return -1;
		} else if (pid == 0) {
			close(fd[0]);
			if (fd[1] != STDIN_FILENO &&
				dup2(fd[1], STDIN_FILENO) != STDIN_FILENO)
				err_sys("dup2");

			if (fd[1] != STDOUT_FILENO &&
				dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
				err_sys("dup2");

			if (execl("./opend_v1", "opend_v1", (char*)0) < 0)
				err_sys("execl");
		}
		close(fd[1]);
	}
	sprintf(buf, " %d", oflag);
	iov[0].iov_base = CL_OPEN " ";
	iov[0].iov_len = strlen(CL_OPEN) + 1;
	iov[1].iov_base = name;
	iov[1].iov_len = strlen(name);
	iov[2].iov_base = buf;
	iov[2].iov_len = strlen(buf) + 1;
	len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
	if (writev(fd[0], &iov[0], 3) != len) {
		err_ret("write");
		return -1;
	}

	return my_recv_fd(fd[0], write);
}

/////////////////////////////////////
static char errmsg[MAXLINE];
static int oflag;
static char *pathname;

static int cli_args(int argc, char **argv);
static void handle_request(char *buf, int nread, int fd);
static int my_buf_args(char *buf, int(*optfunc)(int, char**));

///////////////////////////
int test_open_server_v1()
{
	int nread;
	char buf[MAXLINE];

	for (;;) {
		if ((nread = read(STDIN_FILENO, buf, MAXLINE)) < 0)
			err_sys("read");
		else if (nread == 0)
			break;
		handle_request(buf, nread, STDOUT_FILENO);
	}
	exit(0);
}

static void handle_request(char *buf, int nread, int fd)
{
	int newfd;

	if (buf[nread - 1] != 0) {
		snprintf(errmsg, MAXLINE - 1, "request not null terminated: %*.*s\n",
			nread, nread, buf);
		my_send_err(fd, -1, errmsg);
		return;
	}

	if (my_buf_args(buf, cli_args) < 0) {
		my_send_err(fd, -1, errmsg);
		return;
	}

	if ((newfd = open(pathname, oflag)) < 0) {
		snprintf(errmsg, MAXLINE - 1, "can't open %s: %s\n", pathname, strerror(errno));
		my_send_err(fd, -1, errmsg);
		return;
	}

	if (my_send_fd(fd, newfd) < 0)
		err_sys("send_fd");

	close(newfd);
}

#define		MAXARGC		50
#define		WHITE		" \t\n"

static int my_buf_args(char *buf, int(*optfunc)(int, char**))
{
	char *ptr, *argv[MAXARGC];
	int argc;

	if (strtok(buf, WHITE) == NULL)
		return -1;

	argv[argc = 0] = buf;
	while ((ptr = strtok(NULL, WHITE)) != NULL) {
		if (++argc >= MAXARGC - 1)
			return -1;
		argv[argc] = ptr;
	}
	argv[++argc] = NULL;

	return(*optfunc)(argc, argv);
}

static int cli_args(int argc, char **argv)
{
	if (argc != 3 || strcmp(argv[0], CL_OPEN) != 0) {
		strcpy(errmsg, "Usage: <pathname> <oflag>\n");
		return -1;
	}

	pathname = argv[1];
	oflag = atoi(argv[2]);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//open Server version 2

#define		CS_OPEN		"/tmp/opend.socket"

static int csopen_v2(char *name, int oflag);

///////////////////////////////////////////
int test_open_server_client_v2()
{
	int n, fd;
	char buf[BUFFSIZE];
	char line[MAXLINE];

	while (fgets(line, MAXLINE, stdin) != NULL) {
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;

		if (strlen(line) == 0)
			continue;

		if ((fd = csopen_v2(line, O_RDONLY)) < 0) {
			printf("csopen failed!\n");
			continue;
		}

		while ((n = read(fd, buf, BUFFSIZE)) > 0) {
			if (write(STDOUT_FILENO, buf, n) != n)
				err_sys("write");
		}

		if (n < 0)
			err_sys("read");
		close(fd);
	}

	exit(0);
}

static int csopen_v2(char *name, int oflag)
{
	int len;
	char buf[12];
	struct iovec iov[3];
	static int csfd = -1;

	if (csfd < 0) {
		if ((csfd = my_cli_conn(CS_OPEN)) < 0) {
			err_ret("cli_conn");
			return -1;
		}
	}

	sprintf(buf, " %d", oflag);
	iov[0].iov_base = CL_OPEN " ";
	iov[0].iov_len = strlen(CL_OPEN) + 1;
	iov[1].iov_base = name;
	iov[1].iov_len = strlen(name);
	iov[2].iov_base = buf;
	iov[2].iov_len = strlen(buf) + 1;
	len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
	if (writev(csfd, &iov[0], 3) != len) {
		err_ret("writev");
		return -1;
	}

	return my_recv_fd(csfd, write);
}

//////////////////////////////
typedef struct {
	int fd;
	uid_t uid;
}Client;

#define		NALLOC		10

static int debug;
int log_to_stderr;

Client *client = NULL;
static int client_size;

static void client_alloc();
static int client_add(int fd, uid_t uid);
static void client_del(int fd);

static void loop_select(void);
static void loop_poll(void);
static void handle_request_v2(char* buf, int nread, int clifd, uid_t uid);

/////////////////////////////
int test_open_server_v2(int argc, char *argv[])
{
	int c;
	char *cmd_args[3];

	cmd_args[0] = argv[0];
	cmd_args[1] = "-d";
	cmd_args[2] = 0;

	argc = 2;
	argv = cmd_args;

	log_open("open.serv", LOG_PID, LOG_USER);

	opterr = 0;
	while ((c = getopt(argc, argv, "d")) != EOF) {
		switch (c) {
		case 'd':
			debug = log_to_stderr = 1;
			break;

		case '?':
			err_quit("unrecongnized option: -%c", optopt);
		}
	}

	if (debug == 0)
		my_daemonize("opend_v2");

	//loop_select();
	loop_poll();

	return 0;
}

static void client_alloc()
{
	int i;

	if (client == NULL)
		client = malloc(NALLOC * sizeof(Client));
	else
		client = realloc(client, (client_size+NALLOC) * sizeof(Client));
	
	if (client == NULL)
		err_sys("can't alloc for client array");

	for (i = client_size; i < client_size + NALLOC; ++i)
		client[i].fd = -1;

	client_size += NALLOC;
}

static int client_add(int fd, uid_t uid)
{
	int i;

	if (client == NULL)
		client_alloc();

again:
	for (i = 0; i < client_size; ++i) {
		if (client[i].fd == -1) {
			client[i].fd = fd;
			client[i].uid = uid;
			return i;
		}
	}

	client_alloc();
	goto again;
}

static void client_del(int fd)
{
	int i;

	for (i = 0; i < client_size; ++i) {
		if (client[i].fd == fd) {
			client[i].fd = -1;
			return;
		}
	}
	log_quit("can't find client entry for fd %d", fd);
}

static void loop_select(void)
{
	int i, n, maxfd, maxi, listenfd, clifd, nread;
	char buf[MAXLINE];
	uid_t uid;
	fd_set rset, allset;

	FD_ZERO(&allset);
	if ((listenfd = my_serv_listen(CS_OPEN)) < 0)
		log_sys("serv_listen");
	FD_SET(listenfd, &allset);
	maxfd = listenfd;
	maxi = -1;

	for (;;) {
		rset = allset;
		if ((n = select(maxfd+1, &rset, NULL, NULL, NULL)) < 0)
			log_sys("select");

		if (FD_ISSET(listenfd, &rset)) {
			if ((clifd = my_serv_accept(listenfd, &uid)) < 0)
				log_sys("serv_accept error: %d", clifd);
			i = client_add(clifd, uid);
			FD_SET(clifd, &allset);
			if (clifd > maxfd)
				maxfd = clifd;
			if (i > maxi)
				maxi = i;
			log_msg("new connection: uid %d, fd %d", uid, clifd);
			continue;				//如果其他fd当前可读，重新调用select会立即返回
		}

		for (i = 0; i <= maxi; ++i) {
			if ((clifd = client[i].fd) < 0)
				continue;

			if (FD_ISSET(clifd, &rset)) {
				if ((nread = read(clifd, buf, MAXLINE)) < 0) {
					log_sys("read error on fd: %d", clifd);
				} else if (nread == 0) {
					log_msg("closed: uid %d, fd %d", uid, clifd);
					client_del(clifd);
					FD_CLR(clifd, &allset);
					close(clifd);
				} else {
					handle_request_v2(buf, nread, clifd, client[i].uid);
				}

			}
		}
	}
}

static struct pollfd* grow_pollfd(struct pollfd *pfd, int *maxfd)
{
	int i;
	int oldmax = *maxfd;
	int newmax = oldmax + NALLOC;

	if ((pfd = realloc(pfd, newmax * sizeof(struct pollfd))) == NULL)
		err_sys("realloc error");
	for (i = oldmax; i < newmax; ++i) {
		pfd[i].fd = -1;
		pfd[i].events = POLLIN;
		pfd[i].revents = 0;
	}
	*maxfd = newmax;

	return pfd;
}

static void loop_poll(void)
{
	int i, listenfd, clifd, nread;
	char buf[MAXLINE];
	uid_t uid;
	struct pollfd *pollfd;
	int numfd = 1;
	int maxfd = NALLOC;

	if ((pollfd = malloc(NALLOC * sizeof(struct pollfd))) == NULL)
		err_sys("malloc");

	for (i = 0; i < NALLOC; ++i) {
		pollfd[i].fd = -1;
		pollfd[i].events = POLLIN;
		pollfd[i].revents = 0;
	}

	if ((listenfd = serv_listen(CS_OPEN)) < 0)
		err_sys("serv_listen");
	client_add(listenfd, 0);
	pollfd[0].fd = listenfd;

	for (;;) {
		if (poll(pollfd, numfd, -1) < 0) {
			err_sys("poll");
		}

		if (pollfd[0].revents & POLLIN) {
			clifd = my_serv_accept(listenfd, &uid);
			if (clifd < 0)
				log_sys("serv_accept");
			client_add(clifd, uid);

			if (numfd >= maxfd) {
				pollfd = grow_pollfd(pollfd, &maxfd);
			}
			pollfd[numfd].fd = clifd;
			pollfd[numfd].events = POLLIN;
			pollfd[numfd].revents = 0;
			numfd++;
			log_msg("new connection: uid %d, fd %d", uid, clifd);
		}

		for (i = 1; i < numfd; ++i) {
			if (pollfd[i].revents & POLLHUP) {
				goto hungup;
			} else if (pollfd[i].revents & POLLIN) {
				if ((nread= read(clifd, buf, MAXLINE)) < 0) {
					log_sys("read error on fd %d", pollfd[i].fd);
				} else if (nread == 0) {
hungup:
					log_msg("closed: uid %d, fd %d", client[i].uid, pollfd[i].fd);		//client 和 i的对应关系有问题
					client_del(pollfd[i].fd);
					close(pollfd[i].fd);
					if (i < numfd -1) {
						pollfd[i].fd = pollfd[numfd - 1].fd;
						pollfd[i].events = pollfd[numfd - 1].events;
						pollfd[i].revents = pollfd[numfd - 1].revents;
						i--;		//recheck this entry
					}
					numfd--;
				} else {
					handle_request_v2(buf, nread, clifd, client[i].uid);
				}
			}
		}
	}
}

static void handle_request_v2(char* buf, int nread, int clifd, uid_t uid)
{
	int newfd;
	struct passwd *psd;

	if (buf[nread-1] != 0) {
		snprintf(errmsg, MAXLINE-1, "request from uid %d not null terminated: %*.*s\n",
			uid, nread, nread, buf);
		send_err(clifd, -1, errmsg);
		return;
	}

	psd = getpwuid(uid);
	log_msg("request from uid %d %s: \"%s\" ", uid,psd?psd->pw_name:"", buf);
	if (buf_args(buf, cli_args) < 0) {
		send_err(clifd, -1, errmsg);
		log_msg(errmsg);
		return;
	}

	if ((newfd = open(pathname, oflag)) < 0) {
		snprintf(errmsg, MAXLINE-1, "can't open %s: %s\n", pathname, strerror(errno));
		send_err(clifd, -1, errmsg);
		log_msg(errmsg);
		return;
	}

	if (my_send_fd(clifd, newfd) < 0)
		log_sys("send_fd");
	log_msg("sent fd %d over fd %d for %s", newfd, clifd, pathname);
	close(newfd);
}
