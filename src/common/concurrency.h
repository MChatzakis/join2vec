#ifndef _CONCURENCY_H_
#define _CONCURRENCY_H_

#include <sched.h>
#include <pthread.h>
#include <stdio.h>

#define MAX_THREADS 32
#define COORDINATOR_THREAD 0

void thread_pin(int pid, int max_threads);

#endif