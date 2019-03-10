/*
Consider the following child processes operations:
Process 1 Process 2 Process 3
printf(“Sistemas ”); printf(“de ”); printf(“Computadores -”);
printf(“a ”); printf(“melhor “); printf(“disciplina! ”);
Add operations on semaphores such that the result printed is: “Sistemas de Computadores – a melhor disciplina!”.
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

#define NPROCESSOS 3

int main(int argc, char *argv[]) {

    int idp,j;
    int sem_flag_pais=NPROCESSOS;
    pid_t pid;
    sem_t *semaforo_filhos[NPROCESSOS];
    sem_t *semaforo_pais[NPROCESSOS];

    char c[9];


    // criar o semáforo com valor = 0
    for(j=0;j<NPROCESSOS;j++){
    	//semaforos para os filhos
    	sprintf(c, "semaforo_FILHOS%d", j);
    	if((semaforo_filhos[j]=sem_open(c, O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
    		perror("No sem_open	() | segunda tentativa para criar o semaforo para filhos");
    		if((sem_unlink(c)<0)){
    				perror("No sem_destroy");
    				if((semaforo_filhos[j]=sem_open(c, O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
    					perror("segunda tentativa falhou");
    					exit(1);
    				}
    				else printf("sem_open bem sucedido");
    			}

    	}
    	//semaforos para os pais execepto o original
    	sprintf(c, "semaforo_PAIS%d", j);
    	    	if((semaforo_pais[j]=sem_open(c, O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
    	    		perror("No sem_open	() | segunda tentativa para criar o semaforo para pais");
    	    		if((sem_unlink(c)<0)){
    	    				perror("No sem_destroy");
    	    				if((semaforo_pais[j]=sem_open(c, O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
    	    					perror("segunda tentativa falhou");
    	    					exit(1);
    	    				}
    	    				else printf("sem_open bem sucedido");
    	    			}
    	    	}
    }
	 //incrementar o semaforo do primeiro filho
	 sem_post(semaforo_filhos[0]);

	for(idp=0;idp<NPROCESSOS;idp++){
		pid=fork();
		if(pid ==-1){	//verificação do erro do fork
			perror("Fork falhou");
		}
		//codigo para os processos novos (filhos)
		if(pid==0){
			sem_wait(semaforo_filhos[idp]);						//semaforo para filho
			switch(idp){
				case 0:{ sprintf(c, "sistemas "); break;}  		// mensagem
				case 1:{ sprintf(c, "de "); break;}  			// mensagem
				case 2:{ sprintf(c, "computadores "); break;}	// mensagem
			}

			printf("%s",c);
			sem_post(semaforo_pais[idp]);					 //desbloqueia o primeiro pai
			sem_flag_pais++;

		}
		else if(pid>0){
			sem_wait(semaforo_pais[idp]);				//pai fica à espera que o ultimo processo filho o deixe continuiar
			switch(idp){
				case 0:{ sprintf(c, "- a "); break;}  			// mensagem
				case 1:{ sprintf(c, "melhor "); break;}  		// mensagem
				case 2:{ sprintf(c, "disciplina "); break;}  	// mensagem
			}
			printf("%s",c);
			sem_post(semaforo_filhos[idp+1]);
		}

	}
	//destruir o semaforo --talvez o ultimo pai deva destruir os semaforo
	if(idp==NPROCESSOS){
		for(j=0;j<NPROCESSOS;j++){
			sem_wait(semaforo_filhos[j]);
			sprintf(c, "semaforo_FILHOS%d", j);  // puts string into buffer
			if((sem_unlink(c)<0)) perror("No sem_destroy Filhos");
			sprintf(c, "semaforo_PAIS%d", j);  // puts string into buffer
			if((sem_unlink(c)<0)) perror("No sem_destroy Pais");
		}
	}
    return 0;
}
