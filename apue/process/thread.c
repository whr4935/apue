#include "process.h"
#include <includes.h>
#include <pthread.h>

int TEST_thread()
{
	test_pthread_create();

	return 0;
}

static void clean_up(void *arg)
{
	printf("clean up: %s\n", (char*)arg);
}

static void* thr_func(void *arg)
{
	static int v = 0x12345678;

	printf("in thread, v = %#x\n", v);

	pthread_cleanup_push(clean_up, "first cleaup function");
	pthread_cleanup_push(clean_up, "second cleaup function");
	
//	pthread_exit((void*)&v);
	sleep(10);

	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	
	err_msg("exit...");
//	exit(1);

	for (;;) {
		sleep(1);
	}

	return (void*)&v;
}

int test_pthread_create()
{
	int ret;
	pthread_t thr_id;
	void* v;

	ret = pthread_create(&thr_id, NULL, thr_func, NULL);
	if (ret != 0) {
		err_msg("pthread_create: %s\n", strerror(ret));
		exit(1);
	}
	sleep(2);

	pthread_cancel(thr_id);

//	ret = pthread_detach(thr_id);
	ret = pthread_join(thr_id, (void**)&v);
	if (ret != 0) {
		err_msg("ptrhread_join: %s\n", strerror(ret));
		exit(1);
	}
//	printf("v = %d\n", *(int*)v);
//	sleep(3);

	return 0;

}