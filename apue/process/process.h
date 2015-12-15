#ifndef _PROCESS_H
#define _PROCESS_H

int TEST_process();

int test_env();
int test_setjump();
int test_rlimit();
int test_fork();
int test_vfork();
int test_abort_core();
int test_segement_fault();
int test_signal();
int test_signal_noreentrant();
int test_sleep1();
int test_sleep2();
int test_alarm();
int test_sigprocmask();
int test_process_group();
int test_daemon();
int test_unblock_write();
int test_record_lock();
int test_selct();
int test_aio();
int test_memmap();

#endif