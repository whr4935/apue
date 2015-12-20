#ifndef _TEST_IPC_H
#define _TEST_IPC_H

int TEST_ipc();

int test_pipe();
int test_pipe_page();
int test_coprocess();
int test_msgqueue();
int test_semaphore();
int test_shm();
int test_posix_semaphore();
int test_network_ipc();
int test_ruptime_server();
int test_ruptime_client();
int test_uptime_server_udp();
int test_uptime_client_udp();

int test_unix_domain_socket();
int test_unix_domain_socket_helper(int argc, char **argv);
int test_unix_socket_bind();

//////////////////////////////////////////////////////////////////////////
#define		CL_OPEN		"open"

int csopen(char *, int);

#endif // !_TEST_IPC_H
