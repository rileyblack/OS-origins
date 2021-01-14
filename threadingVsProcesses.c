#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

int x = 10, y = 20, z = 0; //variables declared and initialized as specified in the assignment

/*
 * DESCRIPTION: Function to compute the thread addition
 *
 * PARAMETERS: void pointer to the parent process id to be used for printing purposes
 *
 * RETURN: N/A
 */
void * sum(void * parent_pid) {
        int * parent_pid_int = (int *) parent_pid; //casting void pointer to integer pointer for printing purposes
        unsigned int child_thread_id = pthread_self(); //retrieve thread id for printing purposes

        printf("parent process (PID %d) created child thread (ID %d).\n\n", *parent_pid_int, child_thread_id);
        printf("child thread (ID %d) is preforming the task.\n\n", child_thread_id);

        z = x + y; //preforming addition on global variables

        printf("child thread (ID %d) is now completed. Returning to parent.\n\n", child_thread_id);
}

int main() {

        pid_t process_id, parent_process_id, child_process_id; //variables to hold process ids

        printf("\n\nUSING PROCESSES...\n\n");
        process_id = fork(); //creating process fork


        if (process_id < 0) { //checking if process fork was successful
                printf("process fork unsuccessful.\n\n"); //if unsuccessful, print error message and end program
                exit(1);
        }

        if (process_id > 0) { //case of parent process
                parent_process_id = getpid(); //getting parent process id for printing purposes
                child_process_id = process_id; //getting child process id for printing purposes

                printf("parent process (PID %d) created child process (PID %d).\n\n", parent_process_id, child_process_id);

                wait(NULL); //waiting for child process to complete

                printf("via processes, parent (PID %d) retained the value z = %d.\n\n", parent_process_id, z);

                printf("\n\nUSING THREADING...\n\n");
                pthread_t thread; //declaring new name of thread

                int test = pthread_create(&thread, NULL, sum, &parent_process_id); //creating new thread, storing return from create call

                if (test){ //checking if pthread_create was successful
                        printf("threading unsuccessful.\n\n"); //if unsuccessful, print error message and end program
                        exit(1);
                }

                pthread_join(thread, NULL); //terminating thread

                printf("via threading, parent (PID %d) retained the value z = %d.\n\n\n\n", parent_process_id, z);
        }

        if (process_id == 0) { //case of child process
                child_process_id = getpid(); //getting child process id for printing purposes
                parent_process_id = getppid(); //getting parent process id for printing purposes

                printf("child process (PID %d) with parent process (PID %d) is preforming the task.\n\n", child_process_id, parent_process_id);

                z = x + y; //preforming addition on global variables

                printf("child process (PID %d) with parent process (PID %d) is now completed. Returning to parent.\n\n", child_process_id, parent_process_id);
        }

        return 0;
}