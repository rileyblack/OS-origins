
/*
 * AUTHOR: Riley Black
 * 
 * DESCRIPTION: This program creates a parent process which creates two child processes, and the parent
 * 				waits for child 1 to create its own child. Then, the parent creates child 2 which goes
 * 				on to make a system call to an external program called “external_program.out”
 * 
 * DATE: Sunday, October 6th, 2019 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main() {

	pid_t a, b, c, i, j; //variables to hold process ids at various points in the program

	a = fork(); //creating fork to child 1

	if (a < 0) { //checking if fork to child 1 was successful
		printf("child_1 fork unsuccessful.\n\n");
		exit(1);
	}

	if (a > 0) { //case of parent process
		i = getpid(); //getting parent process id for printing purposes
		printf("\n\nparent (PID %d) created child_1 (PID %d).\n\n", i, a);
		printf("parent (PID %d) is waiting for child_1 (PID %d) to complete before creating child_2.\n\n", i, a);
		wait(NULL); //waiting for child 1 process to complete

		c = fork(); //one child 1 process has concluded, creating fork to child 2

		if (c < 0) { //checking if fork to child 2 was successful
			printf("child_2 fork unsuccessful.\n\n");
			exit(1);
		}

		if (c > 0) { //case of parent process
			printf("parent (PID %d) created child_2 (PID %d).\n\n", i, c);
			wait(NULL); //wait is here so that the output is not skewed by the terminal line thing
		}

		if (c == 0) { //case of child 2 process
			i = getpid(); //getting child 2 process id for printing purposes
			j = getppid(); //getting parent of child 2 process id for printing purposes
			printf("child_2 (PID %d) with parent named 'parent' (PID %d) is calling external program 'external_program.out' and leaving child_2...\n\nFrom the external program: \n\n", i, j);
			execl("external_program.out", NULL); //replacing child 2 with external_program.out
		}
	}
	if (a == 0) { //case of child 1 process
		i = getpid(); //getting child 1 process id for printing purposes
		j = getppid(); //getting parent of child 1 process id for printing purposes

		b = fork(); //creating fork to child 1.1

		if (b < 0) { //checking if fork to child 1.1 was successful
			printf("child 1.1 fork unsuccessful.\n\n");
			exit(1);
		}

		if (b > 0) { //case of child 1 process
			printf("child_1 (PID %d) with parent named 'parent' (PID %d) created child_1.1 (PID %d).\n\n", i, j, b);
			printf("child_1 (PID %d) with parent named 'parent' (PID %d) is now completed.\n\n", i, j);
		}
	}
	return 0;
}
