#include "process.h"
#include <includes.h>
#include <pthread.h>
#include <limits.h>

int TEST_thread()
{
	test_pthread_create();
//	test_pthread_mutex();
//	test_double_lock_coarse();

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
	sleep(1);

	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	
	//	exit(1);

	err_msg("thread begin enter loop...");
	for (;;) {
		sleep(1);
	}
	err_msg("exit...");
	

	return (void*)&v;
}

int test_pthread_create()
{
	int ret;
	pthread_t thr_id;
	void* v;
	pthread_attr_t attr;
	int guardSize;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	ret = pthread_attr_getguardsize(&attr, &guardSize);
	ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN*2);
	ret = pthread_attr_getguardsize(&attr, &guardSize);

	ret = pthread_create(&thr_id, &attr, thr_func, NULL);
	if (ret != 0) {
		err_msg("pthread_create: %s\n", strerror(ret));
		exit(1);
	}
	ret = pthread_attr_destroy(&attr);
	if (ret != 0) {
		err_msg("pthread attr destroy failed: %s\n", strerror(ret));
		exit(1);
	}

	sleep(2);

	pthread_cancel(thr_id);
//	sleep(5);

//	ret = pthread_detach(thr_id);
	ret = pthread_join(thr_id, (void**)&v);
	if (ret != 0) {
		err_msg("ptrhread_join: %s\n", strerror(ret));
		exit(1);
	}
	if (v != PTHREAD_CANCELED)
		printf("v = %d\n", *(int*)v);
//	sleep(3);

	return 0;

}

static int mutex_test_var;

static void* mutex_thr(void *arg)
{
	pthread_mutex_t *p_mutex = (pthread_mutex_t*)arg;

	for (;;) {
		sleep(1);

		pthread_cleanup_push(pthread_mutex_unlock, p_mutex);
		pthread_mutex_lock(p_mutex);
		mutex_test_var++;
		printf("[mutex thread] mutex_test_var = %d\n", mutex_test_var);

		sleep(10);
		pthread_mutex_unlock(p_mutex);
		pthread_cleanup_pop(0);
	}
}

int test_pthread_mutex()
{
	int ret;
	pthread_t t_id;
	int count = 0;
	/*static*/ pthread_mutex_t s_mutex /*PTHREAD_MUTEX_INITIALIZER*/;

	pthread_mutex_init(&s_mutex, NULL);

	pthread_mutex_lock(&s_mutex);
	if ((ret = pthread_mutex_trylock(&s_mutex)) != RT_SUCCESS) {
		err_msg("mutex deadlock: %s\n", strerror(ret));
		return -1;
	}

	printf("hello, I'm safe!\n");

	pthread_mutex_unlock(&s_mutex);
	pthread_mutex_unlock(&s_mutex);


	ret = pthread_create(&t_id, NULL, mutex_thr, &s_mutex);
	if (ret != RT_SUCCESS) {
		err_msg("pthread create failed: %s\n", strerror(ret));
		exit(1);
	}

	for (;;) {
		sleep(1);

// 		pthread_mutex_lock(&s_mutex);
// 		mutex_test_var--;
// 		printf("[main thread] mutex_test_var = %d\n", mutex_test_var);
// 		pthread_mutex_unlock(&s_mutex);

		if (++count >= 2) {
			printf("test finished, exit...\n");
			break;
		}
	}

	pthread_cancel(t_id);
	sleep(1);
	if((ret = pthread_mutex_trylock(&s_mutex)) != RT_SUCCESS) {
		err_msg("mutex deadlock: %s\n", strerror(ret));
		return -1;
	}
	printf("mutex trylock success!\n");
	pthread_mutex_unlock(&s_mutex);


	pthread_mutex_lock(&s_mutex);
	printf("hello, I'm safe!\n");

	pthread_mutex_unlock(&s_mutex);

	return 0;

}

//////////////////////////////////////////////////////////////////////////
#define		NHASH		5
#define		HASH(id)	(((unsigned long)id) % NHASH)

struct foo *fh[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
	int					f_count;
	pthread_mutex_t		f_lock;
	int					f_id;
	struct	foo			*f_next;
	int					val;
};

struct foo* foo_alloc(int id)
{
	struct foo *fp = NULL;
	int			idx;

	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;
		fp->f_id = id;
		if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
			free(fp);
			return NULL;
		}
		idx = HASH(id);
		pthread_mutex_lock(&hashlock);
		fp->f_next = fh[idx];
		fh[idx] = fp;
		pthread_mutex_lock(&fp->f_lock);
		pthread_mutex_unlock(&hashlock);
		fp->val = id;
		pthread_mutex_unlock(&fp->f_lock);
	}

	return fp;
}

void foo_hold(struct foo *fp)
{
	pthread_mutex_lock(&hashlock);
	fp->f_count++;
	pthread_mutex_unlock(&hashlock);
}

struct foo* foo_find(int id)
{
	struct foo* fp;
	pthread_mutex_lock(&hashlock);
	for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
		if (fp->f_id == id) {
			fp->f_count++;
			break;
		}
	}
	pthread_mutex_unlock(&hashlock);

	return fp;
}

void foo_rele(struct foo *fp)
{
	struct foo *tfp;
	int idx;

	pthread_mutex_lock(&hashlock);
	if (--fp->f_count == 0) {
		idx = HASH(fp->f_id);
		tfp = fh[idx];
		if (tfp == fp) {
			fh[idx] = fp->f_next;
		} else {
			while (tfp->f_next != fp) {
				tfp = tfp->f_next;
			}
			tfp->f_next = fp->f_next;
		}
		pthread_mutex_unlock(&hashlock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
		pthread_mutex_unlock(&hashlock);
	}
}

int test_double_lock_coarse()
{
	int i;
	struct foo *fp;

	for (i = 0; i < 10; ++i) {
		fp = foo_alloc(i);
	}

	fp = foo_find(2);
	printf("id(0) var = %d\n", fp->val);
	foo_rele(fp);

	foo_rele(fp);


	return 0;
}


//////////////////////////////////////////////////////////////////////////
