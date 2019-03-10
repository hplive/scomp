#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

/*
 * 6. Given the following code.
 *
 * c) Change the code so that the parent process waits for child processes with an even PID.
 *
 */
int main(void){
	int i,number, count=0,status, auxpid;
	pid_t p[4];
	for(i=0;i<4;i++){
		p[i] = fork();
		if (p[i] == 0){
			number = (int) getpid();
			if(number%2==0){
				printf("Even: %d\n",getpid());
				sleep(i);
				count++;
				exit(count);
			}
		}
		if(p[i]>0){				//codigo para o pai
				do{							//esperar que o filho termine
					auxpid=waitpid(p[i],&status, WNOHANG);
					if (auxpid==-1){
						perror("Erro em waitpid");
						exit(-1);
					}
				}while(auxpid==0);
				if(WIFEXITED(status)){
					printf("Pai: o filho com pid par foi encontrado %d vezes \n",WEXITSTATUS(status));
				}
	}

	}
	printf("This is the end.\n");
	return 0;
}
