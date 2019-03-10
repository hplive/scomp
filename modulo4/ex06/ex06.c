/*
5. Implement a program that creates a new process. Use semaphores to synchronize their actions.
• The child process writes on screen the message “I’m the child";
• The father process writes on scree the message "I’m the father";
• The child process should be (always) the first to write the message on screen.
*/
/*
 * 6. Change the program above in order to write 15 messages, alternating between father and child.
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

int main(int argc, char *argv[]) {
	/* semaforos usados para sincronizar */
	sem_t *semaforopai,*semaforofilho;
	int r; /* utilizado para guardar o valor retornado */

	pid_t p;

	int contadormsg=10,estado;

	 /* semaforo "pai" com valor = 0 */
	if ((semaforopai = sem_open("semaforopai", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("No sem_open()");
		exit(1);
	}
	 /* semaforo "filho" com valor = 1, usado implementar o acesso exclusivo a escrita */
	if ((semaforofilho = sem_open("semaforofilho", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("No sem_open()");
		exit(1);
	}

	p=fork();
	if(p==-1)
	{
		perror("for () Erro");
	}
	while(contadormsg!=0){
		if(p==0) // Filho
		{
			sem_wait(semaforofilho); // espera, decrementa o semaforofilho
			printf(" I’m the child %d \n",contadormsg);
			contadormsg--;
			sem_post(semaforopai); // incrementar o semaforopai
		}
		if(p>0){ // Pai
			sem_wait(semaforopai); // espera, decrementa o semaforopai
			printf("I’m the father %d \n",contadormsg);
			contadormsg--;
			sem_post(semaforofilho); // incrementar o semaforofilho
		}
	}

	/* destruir semaforo pai */
	r = sem_destroy(semaforopai);
	if (r < 0) {
		perror("sem_destroy() erro semaforopai");
		exit(1);
	}
	/* destruir semaforo filho */
	r = sem_destroy(semaforofilho);
	if (r < 0) {
		perror("sem_destroy() erro semaforofilho");
		exit(1);
	}

	/* apagar semaforo pai */
	r = sem_unlink("semaforopai");
	if (r < 0) {
	    perror("No sem_destroy()");
	    exit(1);
	}
	/* apagar semaforo filho*/
	r = sem_unlink("semaforofilho");
	if (r < 0) {
	    perror("No sem_destroy()");
	    exit(1);
	}

	return 0;

}
