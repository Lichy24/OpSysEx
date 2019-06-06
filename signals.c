#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>


int hold;
void sigCathcher(int sig_num) {
	printf("PID %d caught one\n",getpid());
	kill(getpid(), SIGTERM);
}
void sigTerm(int sig_num) {
	sleep(hold);
	printf("Process %d is dead\n", getpid());
	exit(0);
}
int main(void)
{
	signal(SIGTERM, sigTerm);
	signal(SIGINT, sigCathcher);
	pid_t p;
	for (int i = 0; i < 5 && p != 0; i++)
	{
		p = fork();
		if (p == 0) {
			printf("PID %d ready\n", getpid());
			hold = i;
			sleep(4-i);//make sure all will be ready befor execute
		}
	}
	if (p == 0) {
		kill(getpid(), SIGINT);
		while (1);
	}
	else if (p > 0) {
		sleep(11);
		return 0;
	}
}
