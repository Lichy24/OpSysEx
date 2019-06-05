#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>

#include "md5.h"
#define MSGSIZE1 21// 20 + 1 '\0'
#define MSGSIZE2 33//32 + 1 '\0'

int check = 0,gotSig = 1;
char str_en[MSGSIZE2];

void sigint(int sig_num) {//parent function
	signal(SIGINT, sigint);
	if (strlen(str_en) == 32)
		check = 1;
	gotSig = 0;
}
void sigDead(int sig_num) {//kill function
	signal(SIGTERM, sigDead);
	exit(0);
}
int main(int argc, char* argv[]) {

	signal(SIGINT, sigint);//set signal
	signal(SIGTERM, sigDead);//set signal
	int fd1[2]; // Used to store two ends of first pipe 
	int fd2[2]; // Used to store two ends of second pipe 
	pid_t p;//pid of process
	if (pipe(fd1) == -1)//create pipe 1
	{
		fprintf(stderr, "Pipe Failed");
		return 1;
	}
	if (pipe(fd2) == -1)//create pipe 2
	{
		fprintf(stderr, "Pipe Failed");
		return 1;
	}
	p = fork();//create child
	if (p < 0)//failed to create child
	{
		fprintf(stderr, "fork Failed");
		return 1;
	}
	else if (p > 0)//parent process
	{
		close(fd1[0]); // Close reading end of first pipe 

		// Write input string and close writing end of first 
		// pipe. 
		write(fd1[1], argv[1], MSGSIZE1);//write to first pipe the given string from user
		close(fd1[1]);//Close writing end of first pipe

		close(fd2[1]); // Close writing end of second pipe 
		// Read string from child, print it and close 
		// reading end.
		read(fd2[0], str_en, MSGSIZE2);
		close(fd2[0]);//Close read of second pipe from parent
		while (gotSig);
		if (check) {//if true
			printf("plain text: %s\n", argv[1]);
			printf("encrypted by process %d : %s\n",p,str_en);//print the given string in md5 enycrption
			kill(p, SIGTERM);//kill child
		}
	}

	// child process 
	else
	{
		close(fd1[1]); // Close writing end of first pipe 
		char str[MSGSIZE1];//create string holder
		read(fd1[0], str, MSGSIZE1);//read from first pipe the string
		// Close both reading ends 
		close(fd1[0]);//close first pipe read from child
		close(fd2[0]);//close second pipe read from child
		write(fd2[1], md5(str).c_str(), MSGSIZE2);//write to second pipe the encrypted string
		close(fd2[1]);//close second pipe write from child
		kill(getppid(),SIGINT);//send signal to parent
		while (1);//while not die
	}
	return 0;
	
}
