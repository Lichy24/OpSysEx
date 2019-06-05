#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>

void sigCathcher(int sig_num) {
	printf("PID %d caught one\n",getpid());
	kill(getpid(), SIGTERM);
}
void sigTerm(int sig_num) {
	printf("Process %d is dead\n", getpid());
	exit(0);
}
int main(void)
{
	signal(SIGTERM, sigTerm);
	signal(SIGINT, sigCathcher);
	pid_t parent_Pid = getpid();
	pid_t p;
	for (int i = 0; i < 5; i++)
	{
		if (getpid() == parent_Pid) {
			p = fork();
			if (p == 0)
				printf("PID %d ready\n", getpid());
		}
	}
	if (p == 0) {
		kill(getpid(), SIGINT);
		while (1);
	}
}
