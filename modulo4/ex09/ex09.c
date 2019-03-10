#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>

#define NFILHOS 2

typedef struct {
	int comprar_cerveja;
	int comprar_batatas;
} jantar;

int main(int argc, char *argv[]) {

	//apagar lixo semaforos



	int fd; // descritor da memória partilhada
	int r; // utilizado para guardar o valor retornado
	int size= sizeof(jantar);
	jantar *memoria;

	pid_t pid;
	sem_t *semaforo;
	sem_t *barreira;
	char c[9];

	int idp;

	//criar shared memory ********************************************************************************************
	fd = shm_open("/shm_memory", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);	// abrir criar o objeto de memória partilhada
    	if (fd < 0) {
    		perror("No shm_open()");
    		printf("tentaiva de retirar da shared memory");
    		r=shm_unlink("/shm_memory");
    		    	 if (r < 0) { // verifica erro
    		    	     perror("No unlink()");
    		    	     exit(1);
    		    	 }
    		    	 else printf("unlink desfeito\n");

    	}						//se der erro a criar a shared mem
   	    ftruncate(fd, size);												// ajustar o tamanho da mem. partilhada
   	 memoria= (jantar*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mapear a mem. partilhada
    	if (memoria == NULL) { perror("No mmap()"); exit(1); }			//sai se mmap der erro
	//****************************************************************************************************************


	sprintf(c, "semaforo");
	if((semaforo=sem_open(c, O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
		perror("No sem_open	() | segunda tentativa para criar o semaforo");
		if((sem_unlink(c)<0)){
			perror("No sem_destroy");
			if((semaforo=sem_open(c, O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
				perror("segunda tentativa falhou");
				exit(1);
				}
				else printf("sem_open bem sucedido");
		}
  	}
	sprintf(c, "semaforo_barreira");
	if((barreira=sem_open(c, O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("No sem_open	() | segunda tentativa para criar o semaforo");
		if((sem_unlink(c)<0)){
			perror("No sem_destroy");
			if((semaforo=sem_open(c, O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
				perror("segunda tentativa falhou");
				exit(1);
				}
				else printf("sem_open bem sucedido");
		}
  	}

	    //criar X novos processos
	   	for(idp=0;idp<NFILHOS;idp++){
	   		pid=fork();
	   		if(pid ==-1){	//verificação do erro do fork
	   			perror("Fork falhou");
	   		}
	   		//codigo para os processos novos (filhos)
	   		if(pid==0){
	   			//sem_wait(semaforo[idp]);				//decrementa uma unidade no semaforo --neste caso foi inicializado a 1
	   			sem_wait(semaforo);
	   			switch(idp){
	   				case 0 : { memoria->comprar_batatas=1;break; };
	   				case 1 : { memoria->comprar_cerveja=1;break; };
	   				default:break;
	   			}
	   			sem_post(semaforo);						//incrementar semaforo para que o processo pai prossiga
	   			if((memoria->comprar_batatas+memoria->comprar_cerveja)==2){
	   				sem_post(barreira);

	   			}
	   		}
	   		else{
	   			sem_wait(barreira);
	   			sprintf(c, "bebe e come ");   			// puts string into buffer
	   			printf("%s",c);
	   			//sem_post(barreira);

	   		}

	   	}


		sem_wait(semaforo);
	   	sprintf(c, "semaforo");   // puts string into buffer
	   	if((sem_unlink(c)<0)){
	   		perror("No sem_destroy ou já destruido");
	   		}

	   	sprintf(c, "barreira");   // puts string into buffer
	   		   	if((sem_unlink(c)<0)){
	   		   		perror("No sem_destroy ou já destruido");
	   		   		}
	    // o leitor( pai ) apaga a memoria partilhada do sistema
	      r=munmap(memoria, size); // desfaz mapeamento
	      if (r < 0) { // verifica erro
	             perror("No munmap()");
	             exit(1);
	      }
	      else{
	     	 //printf("mapeamento desfeito\n");
	     	 r=shm_unlink("/shm_memory");
	     	 if (r < 0) { // verifica erro
	     	     perror("No unlink()");
	     	     exit(1);
	     	 }
	     	 //else printf("unlink desfeito\n");
	     	}


	   	return 0;



}
