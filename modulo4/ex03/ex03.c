
/*3.
 * Implement a program that should behave as follows:
• Guarantee mutual exclusion to the shared memory;
• Add, to the shared memory, a text line with the following information: "I’m the Father - with PID X", where X
should be replaced by the PID of the process;
• Wait N seconds (from 1 to 5 seconds);
Test the solution by executing several concurrent instances of your code.
Note: Consider that you have a shared memory area with 50 strings, each string having 80 characters.
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

#define BUFFER_SIZE 100
#define CHARACTER 80
#define NPROCESSOS 2
#define N 2

typedef struct{
	char text[BUFFER_SIZE];
	char string[50][CHARACTER];
	pid_t pid_do_pai;
}buffer;

int main(int argc, char *argv[]) {


    int fd; // descritor da memória partilhada
    int r; // utilizado para guardar o valor retornado
    int size= sizeof(buffer);
    buffer *memoria;
    int idp, auxpid, status;
    pid_t pid;
    sem_t *semaforo;
	
	
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
   	 memoria= (buffer*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mapear a mem. partilhada
    	if (memoria == NULL) { perror("No mmap()"); exit(1); }			//sai se mmap der erro
	//****************************************************************************************************************
		
		memoria->pid_do_pai=getpid();

    // criar o semáforo com valor = 1
    	if((semaforo=sem_open("semaforo1", O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
    		perror("No sem_open	() | segunda tentativa para criar o semaforo");
    		if((sem_unlink("semaforo1")<0)){
    				perror("No sem_destroy");
    				if((semaforo=sem_open("semaforo1", O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
    					perror("segunda tentativa falhou");
    				}
    				else printf("sem_open bem sucedido");
    			}
    		exit(1);
    	}

	 //criar X novos processos
	for(idp=0;idp<NPROCESSOS;idp++){
		pid=fork();
		if(pid ==-1){	//verificação do erro do fork
			perror("Fork falhou");
		}
		//codigo para os processos novos (filhos)
		if(pid==0){
			if(idp==0) { sem_post(semaforo);}      //para que seja o primeiro filho a usar a memoria partilhada
			sem_wait(semaforo);						//decrementa uma unidade no semaforo --neste caso foi inicializado a 1
			sprintf(memoria->text, "EU SOU O FILHO E TENHO O PID %d \n", getpid());   // puts string into buffer
			printf("%s",memoria->text);
			sem_post(semaforo);						//incrementar semaforo para que outro processo possa continuar
			sleep(N);								//esperar N segundos como diz o enunciado, podia ter feito com um random de 1 até 5 mas ....please
			exit(0);								//fim de processo filho i
		}
	}

	{ // codigo para o pai
		while(idp!=NPROCESSOS);
		sem_wait(semaforo);						//decrementa uma unidade no semaforo --neste caso foi inicializado a 1
		sprintf(memoria->text, "EU SOU O PAI E TENHO O PID %d \n", getpid());   // puts string into buffer
		printf("%s",memoria->text);
		sem_post(semaforo);						//incrementar semaforo para que outro processo possa continuar
	}

	if((sem_unlink("semaforo1")<0)){
		perror("No sem_destroy");
	}

    do{
    	auxpid=waitpid(pid, &status, WNOHANG);
    }while(auxpid==0);
    // o leitor( pai ) apaga a memoria partilhada do sistema
     r=munmap(memoria, size); // desfaz mapeamento
     if (r < 0) { // verifica erro
            perror("No munmap()");
            exit(1);
     }
     else{
    	 printf("mapeamento desfeito\n");
    	 r=shm_unlink("/shm_memomy");
    	 if (r < 0) { // verifica erro
    	     perror("No unlink()");
    	     exit(1);
    	 }
    	 else printf("unlink desfeito\n");
    	}
	return 0;

}


