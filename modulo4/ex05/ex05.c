/*
5. Implement a program that creates a new process. Use semaphores to synchronize their actions.
• The child process writes on screen the message “I’m the child";
• The father process writes on scree the message "I’m the father";
• The child process should be (always) the first to write the message on screen.
*/

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>

#define NPROCESSOS 2

int main(int argc, char *argv[]) {
	//apagar semaforos
	system("make clear");



    int idp,j;
    pid_t pid;
    sem_t *semaforo[NPROCESSOS];
    char c[100];


    // criar o semáforo com valor = 0
    for(j=0;j<NPROCESSOS;j++){
    	char c[9];
    	sprintf(c, "semaforo%d", j);
    	if((semaforo[j]=sem_open(c, O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
    		perror("No sem_open	() | segunda tentativa para criar o semaforo");
    		if((sem_unlink(c)<0)){
    				perror("No sem_destroy");
    				if((semaforo[j]=sem_open(c, O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
    					perror("segunda tentativa falhou");
    					exit(1);
    				}
    				else printf("sem_open bem sucedido");

    			}

    	}
    }

	 //criar X novos processos
	for(idp=0;idp<NPROCESSOS;idp++){
		pid=fork();
		if(pid ==-1){	//verificação do erro do fork
			perror("Fork falhou");
		}
		//codigo para os processos novos (filhos)
		if(pid==0){
			//sem_wait(semaforo[idp]);						//decrementa uma unidade no semaforo --neste caso foi inicializado a 1
			sprintf(c, "EU SOU O FILHO E TENHO O PID %d e COM PAI %d\n", getpid(), getppid());   // puts string into buffer
			printf("%s",c);
			sem_post(semaforo[idp]);				//incrementar semaforo para que o processo pai prossiga

		}
		else{
			sem_wait(semaforo[idp]);						//pai fica à espera que o processo filho o deixe continuiar
			sprintf(c, "EU SOU O PAI E TENHO O PID %d com FILHO %d\n", getpid(), pid);   // puts string into buffer
			printf("%s",c);
			//sem_post(semaforo[idp]);
			sem_post(semaforo[idp]);

		}
	}

	//destruir o semaforo --talvez o ultimo pai deva destruir os semaforo
	if(idp==NPROCESSOS){
		for(j=0;j<NPROCESSOS;j++){
			sem_wait(semaforo[j]);
			sprintf(c, "semaforo%d", j);   // puts string into buffer
					//printf("%s",c);
					if((sem_unlink(c)<0)){
							perror("No sem_destroy");
					}
		}
		exit(0);


	}


    	return 0;

}
