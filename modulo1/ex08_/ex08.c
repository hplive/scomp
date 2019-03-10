/*
 * Compile and execute the code below, sending the main process to the background
 *  (in Linux, you tell the shell to execute a process in the background by
 *  adding “&” at the end). Observe that the parent process will execute a
loop forever.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

int main()
{
	pid_t p;
	if (fork() == 0) {
		printf("PID = %d\n", getpid()); exit(0);
	}
	if ((p=fork()) == 0) {
		printf("PID = %d\n", getpid()); exit(0);
	}
		printf("Parent PID = %d\n", getpid());
		printf("Waiting... (for PID=%d)\n",p);
		waitpid(p, NULL, 0);
		printf("Enter Loop...\n");
	while (1); /* Infinite loop */
}

/*
 *
 colocando o comando
 ps -ef | grep ex08


a) temos que nos falta só não aparece um pid, o 24961 por exemplo que é o segundo filho, ou
 o filho que o pai fica à espera

isep     24959 24281 88 19:44 pts/21   00:08:06 ./ex08
isep     24960 24959  0 19:44 pts/21   00:00:00 [ex08] <defunct>
isep     24997  3453  0 19:53 pts/4    00:00:00 grep --

c) kill -kill mata não so o processo pai, mas também os que estão agarrados ao mesmo

 */
