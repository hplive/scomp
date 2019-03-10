#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(void)
{
	int i, count=0;
	pid_t p;
	for (i = 0; i < 4; i++) {

		if (fork()==0) {
			//sleep(1); /*sleep(): unistd.h*/
			printf("filho %d count=%d\n",getpid());
			exit(1);
		}

	}

	printf("This is the end.\n");
	return 0;

}

/*a) How many processes will be created by this code? Justify by drawing a process tree that describes the
processes created.
b) What change, if any, would you do to this code so that exactly 4 child processes are created?
c) Change the code so that the parent process waits for child processes with an even PID.
d) Change the code so that the child processes return a number that reflects their creation order (that is, the
first child process returns 1, the second returns 2, …).

a)
|
|
|i=0 fork()_____________________________________
|												|
|i=1 fork()______________________				|_______________
|								|				|				|
|i=2 fork()______________		|_______		|______	_		|_______
|						|		|		|		|		|		|		|
|i=3 fork()____			|___	|___	|___	|___	|__		|__		|__
|				|		|	|	|	|	|	|	|	|	|	|	|	|	|	|
|				|		|	|	|	|	|	|	|	|	|	|	|	|	|	|

serão criados 16 processos

b) colocava um exit no if do filho
*/
