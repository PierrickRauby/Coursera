#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <string.h>
#include <syslog.h>

#define NUM_THREADS 1

typedef struct
{
    int threadIdx;
} threadParams_t;

// POSIX thread declarations and scheduling attributes
pthread_t threads[NUM_THREADS];

/// @brief Function executed by each thread, logs in syslog
void *threadFunc(void *threadp)
{
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello World from Thread!");
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
    // get value of uname -a
    fp = popen("uname -a", "r");
    // handle errors
    if (fp == NULL)
    {
        syslog(LOG_ERR, "[COURSE:1][ASSIGNMENT:1] Failed to run uname -a");
    }
    else
    {
        if (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            // Strip newline, if any
            buffer[strcspn(buffer, "\n")] = 0;
            syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] %s", buffer);
        }
        pclose(fp);
    }

    // Main funciton writes in syslog
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello World from Main!");

    // loop to create all the thread
    for (i = 0; i < NUM_THREADS; i++)
    {
        // create the threads for each to execute threadFunc
        pthread_create(&threads[i],
                       (void *)0,
                       threadFunc,
                       NULL);
    }

    // wait for all thread to complete
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    closelog();

    printf("TEST COMPLETE\n");
}
