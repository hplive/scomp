#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(void)
{
	int i;
	int status;
	pid_t p[4], auxpid;
	for (i = 0; i < 4; i++) {
		p[i]=fork();
		if(p[i]<0){					//se der erro a criar op processo(s)
			perror("fork failed");exit(1);
		}
		if(p[i]>0){					//IF PARA O PAI
			do{
				auxpid=waitpid(p[i],&status, WNOHANG);
				if (auxpid==-1)
				{
					perror("Erro em waitpid");
					exit(-1);
				}
			}while(auxpid==0);
		}
		if (p[i] == 0) 				//codigo do filho
			exit(i+1);			//retorna a orderm correspondente com que foi criado o processo filho


		if(WIFEXITED(status))
			printf("Pai: o filho %d retornou o valor(ordem):%d\n", auxpid,WEXITSTATUS(status));



	}

	printf("This is the end.\n");


	return 0;
}

/*d) Change the code so that the child processes return a number that reflects
 * their creation order (that is, the
first child process returns 1, the second returns 2, …).


*/
