
/*
 * AUTHOR: Riley Black
 * 
 * DESCRIPTION: This program accepts two integer values from the user as command line arguments, then 
 * 				creates a parent process that reads integer 1 (X) and a child process that reads integer
 * 				2 (Y). The child process then sends Y to parent process through a pipe, where the parent 
 * 				process then sums the two and displays the output.
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

int main(int argc, char * argv[]) {

	char c; //character used to pass the Y variable through the pipe
	int fd[2]; //integer array used to read and write from pipe
	pid_t a, i; //variables to hold process ids at various points in the program

	if (pipe(fd) < 0) { //checking if pipe was created successfully
		perror("Pipe error.\n\n");
		exit(0);
	}

	printf("\n\nA pipe is created for communication between parent (PID %d) and child.\n\n", getpid());

	a = fork();

	if (a < 0) { //checking if fork was created successfully
		perror("Fork error.\n\n");
		exit(0);
	}

	if (a > 0) { //case of parent process
		i = getpid(); //getting parent process id for printing purposes
		printf("parent (PID %d) created a child (PID %d).\n\n", i, a);
		char * X = argv[1]; //getting X variable from user input and storing in character X
		printf("parent (PID %d) reading X = %c from the user.\n\n", i, *X);
		
		wait(NULL); //waiting for child to read it's value and pass via pipe

		read(fd[0], &c, 1); //once the child has successfully completed its task, the parent resumes by reading in one character from the pipe

		printf("parent (PID %d) reading Y from the pipe (Y = %c).\n\n", i, c);
		printf("parent (PID %d) adding X + Y = %d.\n\n", i, atoi(X) + atoi(&c)); //atoi converts characters to integers to preform the addition
	}
	else { //case of child process
		i = getpid(); //getting child process id for printing purposes
		char * Y = argv[2]; //getting Y variable from user input and storing in character Y
		printf("child (PID %d) reading Y = %c from the user.\n\n", i, *Y);
		printf("child (PID %d) writing Y into the pipe.\n\n", i);
		write(fd[1], Y, 1); //child writes one character to the pipe
	}
	return 0;
}
