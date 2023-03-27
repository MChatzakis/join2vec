#include "./concurrency.h"

void thread_pin(int pid, int max_threads)
{
    int cpu_id;

    cpu_id = pid % max_threads;
    pthread_setconcurrency(max_threads);

    cpu_set_t mask;
    unsigned int len = sizeof(mask);

    CPU_ZERO(&mask);
    CPU_SET(cpu_id % max_threads, &mask); 

    int ret = sched_setaffinity(0, len, &mask);
    if (ret == -1){
        perror("sched_setaffinity");
    }
    //! todo: read more about pinning
}
