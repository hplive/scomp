#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

/*
 * 5. Write a program that creates two child processes, waits for each one of them to terminate and then outputs their
exit value. The first child process created should wait for 1 second and then exit with a return value of 1. The
second child created should wait for 2 seconds and exit returning 2.
 */

int main(void){

	pid_t pid[2];
	int aux;
	int status;

	int i=0;
	for(i=0;i<2;i++){
		pid[i]=fork();
		if (pid[i]<0)    //se o fork nao for criado e der erro devolve -1
		{
			perror("Erro ao cria o processo\n");
			exit(-1);
		}
		if(pid[i]>0){ //codigo do pai
			printf("eu sou o pai\n");
			do{
				aux=waitpid(pid[i], &status, WNOHANG);
				if (aux==-1)
					 {
						 perror("Erro em waitpid");
						 exit(-1);
					 }
				if (aux == 0)
				{
					printf(".\n");
					sleep(1);
				}
			}while(aux==0);
		} //fim do if para o pai
		if(pid[i]==0){
			printf("eu sou o %dº filho\n",i+1);
			sleep(i);
			exit(i+1);

			}

		if (WIFEXITED(status))
				{
					printf("Pai: o %dº filho retornou o valor:%d\n",i+1,WEXITSTATUS(status));
				}

		}//fim do for
	return 0;
}
