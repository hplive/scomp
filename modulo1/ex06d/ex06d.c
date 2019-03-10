#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

/*
 * 6. Given the following code.
 *
 * Change the code so that the child processes return a number that reflects their creation order (that is, the
 * first child process returns 1, the second returns 2, …)
 */

int main(void){
	int i,status, auxpid;
	pid_t p[4];
	for(i=0;i<4;i++){
		p[i] = fork();
		if(p[i]<0){
			perror("fork failed");
			exit(1);
		}else if(p[i]>0){				//pai
			do{							//espera que o filho termine
				auxpid=waitpid(p[i],&status, WNOHANG);
				if (auxpid==-1){
					perror("Erro em waitpid");
					exit(-1);
				}
			}while(auxpid==0);
		}else if (p[i] == 0){			//filho
				exit(i+1);				//retorna a ordem
		}
		if(WIFEXITED(status))
					printf("Pai: o filho %d retornou o valor(ordem):%d\n", auxpid,WEXITSTATUS(status));
	}
	printf("This is the end.\n");
	return 0;
}
