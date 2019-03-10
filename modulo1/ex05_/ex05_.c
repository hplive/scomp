#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

/*
 * 5. Write a program that creates two child processes,
 * waits for each one of them to terminate and then outputs their exit value.
 * The first child process created should wait for 1 second and then exit
 * with a return value of 1.
 * The second child created should wait for 2 seconds and exit returning 2.
 */

int main(void){
	int n=2, i,val,status;
	pid_t p[n];

	for(i=0;i<n;i++){
		p[i]=fork();
		if(p[i]==-1){
			perror("Fork failed."); //se valor devolvido pelo fork for -1 erro na criação do clone processo
			exit(-1);
		}else if(p[i]>0){ //se valor devolvido pelo fork >0 estamos a trabalhar com o pai
			printf("Pai\n");
			do{
				val=waitpid(p[i], &status, WNOHANG); //espera/bloqueia até que o "filho" p[i] termine
				if (val==-1){
						 perror("Erro em waitpid"); //erro no wait
						 exit(-1);
				}else if (val == 0){   //ainda não terminou
					printf(".\n");
					sleep(1);
				}
			}while(val==0);
		}else if(p[i]==0){//se valor devolvido pelo fork =0 estamos a trabalhar com o filho
					printf("%dº Filho\n",i+1);
					sleep(i+1);
					exit(i+1);
		}
		if(WIFEXITED(status)){
			if(status!=0){
				printf("Pai: o filho nº %d retornou o valor: %d.\n", i+1, WEXITSTATUS(status));
			}
		}
	}
	return 0;
}
