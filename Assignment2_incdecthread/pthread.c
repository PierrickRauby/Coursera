#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <string.h>
#include <syslog.h>

#define NUM_THREADS 128

typedef struct
{
    int threadIdx;
} threadParams_t;

// POSIX thread declarations and scheduling attributes
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];

/// @brief thread function, calculate the sum based on threadIdx and
///        outputs in syslog
void *threadFunc(void *threadp)
{
    int sum = 0;
    int i;
    threadParams_t *threadParams = (threadParams_t *)threadp;
    for (i = 0; i < threadParams->threadIdx; i++)
    {
        sum++;
    }
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:2] Thread idx=%d, sum[1..%d]=%d", threadParams->threadIdx, threadParams->threadIdx, sum);
    return NULL;
}

int main(int argc, char *argv[])
{
    int i;

    // open log
    openlog("pthread", LOG_CONS, LOG_USER);

    // Log uname -a to syslog
    FILE *fp;
    char buffer[512];
    fp = popen("uname -a", "r");
    // handle errors
    if (fp == NULL)
    {
        syslog(LOG_ERR, "[COURSE:1][ASSIGNMENT:2] Failed to run uname -a");
    }
    else
    {
        if (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            // Strip newline, if any
            buffer[strcspn(buffer, "\n")] = 0;
            syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:2] %s", buffer);
        }
        pclose(fp);
    }

    // Create and join threads
    for (i = 0; i < NUM_THREADS; i++)
    {
        threadParams[i].threadIdx = i;            // populate param array for current thread
        pthread_create(&threads[i],               // pointer to thread descriptor
                       (void *)0,                 // use default attributes
                       threadFunc,                // thread function entry point
                       (void *)&(threadParams[i]) // parameters to pass in
        );
    }

    // wait for all thread to complete
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    closelog();

    printf("TEST COMPLETE\n");
}
