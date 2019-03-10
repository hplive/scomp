/*
 * 8. Consider the two following processes:
P1: while true do: print “S”
P2: while true do: print “C”
Add semaphores such that at any moment the number of S or C differs by at most one. The solution should allow
strings such as: “SCCSSCCSCS”
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

typedef struct{
	int contadors;
	int contadorc;
}contador;

int main(int argc, char *argv[]) {
	/* semaforos usados para sincronizar */
	sem_t *semaforopai,*semaforofilho;
	int r,i; /* utilizado para guardar o valor retornado */
	int fd;

	contador *memoria;
	int size= sizeof(contador);
	pid_t p[2];

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
	   	 memoria= (contador*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mapear a mem. partilhada
	    	if (memoria == NULL) { perror("No mmap()"); exit(1); }

	 /*semaforo "pai" com valor = 0 */
	if ((semaforopai = sem_open("semaforopai", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("No sem_open()");
		exit(1);
	}
	  /*semaforo "filho" com valor = 1, usado implementar o acesso exclusivo a escrita*/
	if ((semaforofilho = sem_open("semaforofilho", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("No sem_open()");
		exit(1);
	}


	for(i=0;i<2;i++){
		p[i]=fork();
	}

	if(p[0]==0){// Filho
		while(abs(memoria->contadors-memoria->contadorc)<1){
			sem_wait(semaforofilho); // espera, decrementa o semaforofilho
				printf("S\n");
				memoria->contadors = memoria->contadors+1;
			printf("contador s %d filho \n", memoria->contadors);
			printf("contador c %d filho \n", memoria->contadorc);
			sem_post(semaforopai); // incrementar o semaforo
			//sem_wait(semaforofilho);
		}
	}
	if(p[1]==0){
		printf("contador s %d pai \n", memoria->contadors);
		printf("contador c %d pai \n", memoria->contadorc);
		while(abs(memoria->contadors-memoria->contadorc)==1){
			sem_wait(semaforopai); // espera, decrementa o semaforo
				printf("C\n");
				memoria->contadorc = memoria->contadorc+1;
			sem_post(semaforofilho); // incrementar o semaforofilho
			//sem_wait(semaforopai);
		}
	}



	/* destruir semaforo pai */
	r = sem_destroy(semaforopai);
	if (r < 0) {
		perror("sem_destroy() erro semaforopai");
		exit(1);
	}
	r = sem_destroy(semaforofilho);
		if (r < 0) {
			perror("sem_destroy() erro semaforopai");
			exit(1);
		}

	/* apagar semaforo filho*/
	r = sem_unlink("semaforopai");
	if (r < 0) {
	    perror("No sem_destroy()");
	    exit(1);
	}

	r = sem_unlink("semaforofilho");
	if (r < 0) {
	    perror("No sem_destroy()");
	    exit(1);
	}

	return 0;

}
