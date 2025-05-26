#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>

#define NUM_THREADS 1

typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];


void *threadFunc(void *threadp)
{
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello World from Thread!");
    return NULL;
}


int main (int argc, char *argv[])
{
   int i;

   // open log
   openlog("pthread", LOG_PID | LOG_CONS, LOG_USER);

   // Need to add uname -r info

   syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello World from Main!");

   for(i=0; i < NUM_THREADS; i++)
   {

       pthread_create(&threads[i],   // pointer to thread descriptor
                      (void *)0,     // use default attributes
                      threadFunc, // thread function entry point
                        NULL
                    //   (void *)&(threadParams[i]) // parameters to pass in
                     );

   }

   for(i=0;i<NUM_THREADS;i++)
       pthread_join(threads[i], NULL);

   closelog();

   printf("TEST COMPLETE\n");
}
