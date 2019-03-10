#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
/*
 *
 *Write a program that creates 10 child processes to play the game “Win the pipe!”.
 *There must be only one pipe, shared by all processes.The game’s rules are as follow:
 *
 *a. The parent process fills, each 2 seconds, a structure with a message “Win” and the round’s number (1 to
 *	10) and writes this data in the pipe;
 *
 *b. Each of child processes is trying to read data from the pipe.
 *	The child that succeeds should print the inning message and the round’s number,
 *	ending its execution with an exit value equal to the winning ound’s number;
 *
 *c. The remaining child processes continue to try to read data from the pipe;
 *d. After all child processes terminate, the parent process should print the PID and
 *	the winning round of each child.
*/

#define numProcess 10

typedef struct { //estrutura dados
	char message[10];
	int roundNum;
}valores;

int main(void){
	int i,f, status, auxpid;
	valores e,r;
	char string[]="win";
	pid_t p[numProcess];
	int fd[2];

	time_t t;
	srand((unsigned) time(&t)); //gerar numero aleatório

	/* cria pipe */
	if(pipe(fd)==-1){
		perror("Pipe Falhou\n");
		return 1;
	}

	for(i=0;i<numProcess;i++){ //criar filhos
			p[i]=fork();
			if(p[i] == -1){
				perror("Fork failed.");
				exit(-1);
			}
			if(p[i]>0){ //Pai
				sleep (2);
				close(fd[0]); // fecha o pipe de leitura
				strcpy(e.message, string);
				e.roundNum = i+1;
				write(fd[1],&e,sizeof(valores));
				close(fd[1]); // fecha o pipe de escrita
			}else{ //Filho
				close(fd[1]); //fecha o pipe de escrita
				read(fd[0],&r,sizeof(valores));
				close(fd[0]);
				printf("Filho n.º %d, com o pid = %d, %s  a Ronda nº %d.\n", i+1, getpid(), r.message, r.roundNum);
				exit(r.roundNum);
			}
	}
	for(f=0;f<numProcess;f++){
		do{							//esperar que o filho termine
			auxpid=waitpid(p[f],&status, WNOHANG);
			if (auxpid==-1){
				perror("Erro em waitpid");
				exit(-1);
			}
		}while(auxpid==0);
		if(WIFEXITED(status)){
			printf("Pai: o filho com pid %d venceu a ronda  %d \n",auxpid, WEXITSTATUS(status));
		}
	}
	return 0;
}
