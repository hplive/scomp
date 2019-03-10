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

#define NPROCESSOS 3

typedef struct{
	int filhos_escreveram;
}control;

int main(int argc, char *argv[]) {


    int idp,j;
    pid_t pid;
    sem_t *semaforo[NPROCESSOS*2];
    sem_t *semaforo_main;
    char c[100];

    int size= sizeof(control);
    control *memoria;
    int fd; // descritor da memória partilhada
    int r; // utilizado para guardar o valor retornado

    //criar shared memory ********************************************************************************************
    	fd = shm_open("/shm_memomy", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);	// abrir criar o objeto de memória partilhada
        	if (fd < 0) {
        		perror("No shm_open()");
        		printf("tentaiva de retirar da shared memory");
        		r=shm_unlink("/shm_memomy");
        		    	 if (r < 0) { // verifica erro
        		    	     perror("No unlink()");
        		    	     exit(1);
        		    	 }
        		    	 else printf("unlink desfeito\n");
        		exit(1);
        	}						//se der erro a criar a shared mem
       	    ftruncate(fd, size);												// ajustar o tamanho da mem. partilhada
       	 memoria= (control*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mapear a mem. partilhada
        if (memoria == NULL) { perror("No mmap()"); exit(1); }			//sai se mmap der erro
    	//****************************************************************************************************************

        memoria->filhos_escreveram=0;

    // criar o semáforo com valor = 0
    for(j=0;j<=NPROCESSOS;j++){
    	sprintf(c, "semaforo%d", j);
    	if((semaforo[j]=sem_open(c, O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
    		perror("No sem_open	() | segunda tentativa para criar o semaforo");
    		if((sem_unlink(c)<0)){
    				perror("No sem_destroy");
    				if((semaforo[j]=sem_open(c, O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
    					perror("segunda tentativa falhou");
    					exit(1);
    				}
    				else printf("sem_open bem sucedido");

    			}

    	}
    }
    sprintf(c, "semaforo_main");
	if((semaforo_main=sem_open(c, O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("No sem_open	() | segunda tentativa para criar o semaforo");
		if((sem_unlink(c)<0)){
				perror("No sem_destroy");
				if((semaforo[j]=sem_open(c, O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
					perror("segunda tentativa falhou");
					exit(1);
				}
				else printf("sem_open bem sucedido");

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
			sem_wait(semaforo[idp]);						//decrementa uma unidade no semaforo --neste caso foi inicializado a 1
			sprintf(c, "bla %d",idp);			   // puts string into buffer
			printf("%s",c);
			memoria->filhos_escreveram++;
			sem_post(semaforo[idp+1]);				//incrementar semaforo para que o processo pai prossiga
			if(memoria->filhos_escreveram==3)
				sem_post(semaforo_main);

		}
		else{
			sem_wait(semaforo_main);
			sem_wait(semaforo[idp]);						//pai fica à espera que o processo filho o deixe continuiar
			sprintf(c, "EU SOU O PAI E TENHO O PID %d com FILHO %d\n", getpid(), pid);   // puts string into buffer
			printf("%s",c);
			//sem_post(semaforo[idp]);
			sem_post(semaforo_main);
			sem_post(semaforo[idp+1]);

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
