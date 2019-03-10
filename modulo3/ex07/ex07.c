#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <sys/wait.h>
#include <sys/types.h>
#include "time.h"
#include "random.h"

/*
 * 7. Implement a program similar to 5, but now the child processes send
 * the local maximum using a pipe, shared by all processes.
 */


/* estrutura de dados para a memoria partilhada*/
#define N_MAX 10
#define RANGE 1000

#define SHMEX "/EX07"


typedef struct{
	int maxNumbers[N_MAX];
} shared_data_type;


int main(int argc, char *argv[]){

	time_t t;
	/* Iniciar o gerador de numeros*/
	srand((unsigned) time(&t));
	int fd[2];
	int i, f,r, auxpid, status, maximo=0;
	int vec[RANGE];
	int tamanhoProcura = RANGE/N_MAX;

	//shared_data_type *shared_data; //apontador da shm

	pid_t p[N_MAX];

	/* cria pipe */
	if(pipe(fd)==-1){
		perror("Pipe Falhou\n");
		return 1;
	}

	for(i=0; i<RANGE; i++){
		vec[i]=generateNumber(RANGE);
		//printf("posicao %d numero gerado: %d\n", i+1, vec[i]);
	}

	/* Cria processos */
	for(i=0;i<N_MAX;i++){
		p[i] = fork();
		if(p[i] == -1){
				perror("Fork failed.");
				exit(-1);
		}else if(p[i]> 0){/* PAI */
			sleep(2);
			close(fd[0]); //fecha o pipe de leitura
			for(f=i*tamanhoProcura;f<(i+1)*tamanhoProcura; f++){
				if(maximo<vec[f]){ //procurar o maximo
					maximo = vec[f]; //alocar o maximo
				}
			}
			write(fd[1],&maximo, sizeof(maximo));
			close(fd[1]); //fecha o pipe de escrita
		}else if(p[i] == 0){ /* FILHO */
			close(fd[1]); //fecha o pipe de escrita
			read(fd[0],&r,sizeof(r));
			printf("Filho n.º %d, encontrou o máximo %d.\n", i+1, r);
			close(fd[0]);
			exit(r);
		}

	}

	for(f=0;f<N_MAX;f++){
			do{							//esperar que o filho termine
				auxpid=waitpid(p[f],&status, WNOHANG);
				if (auxpid==-1){
					perror("Erro em waitpid");
					exit(-1);
				}
			}while(auxpid==0);
			if(WIFEXITED(status)){
				printf("Maximo %d\n", WEXITSTATUS(status));
			}
	}
/*

	for(i=0;i<N_MAX;i++){
		if(maximo<shared_data->maxNumbers[i]){
			maximo = shared_data->maxNumbers[i];
		}
	}

	printf("O máximo global é: %d.\n", maximo);


*/

    return 0;
}
