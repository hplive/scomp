#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(void)
{
	int i,aux;
	int status;
	pid_t p;
	for (i = 0; i < 2; i++) {
		p=fork();
		if (p == 0) {
			sleep(1); /*sleep(): unistd.h*/
		}
		//codigo do pai
		if(p%2==0 && p!=0){
			printf("pares %d\n",p);
			do{
				aux=waitpid(p, &status, WNOHANG);
				printf("à espera... %d\n",aux);
			}while(aux==0);
		}
		if(p%2!=0) printf("impar %d\n",p);

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


c) basta colocar esta condição
	if(p%2==0 && p!=0){
			waitpid(p, &status, WNOHANG);
		}
		*/
