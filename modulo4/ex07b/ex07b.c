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
#include <string.h>
#define NPROCESSOS 3

typedef struct {
	char mensagens[6][50];
	int cont;
}buffer;



int main(int argc, char *argv[]) {

	int  i,r;//, cont, v, num;
	//int pid;
	int fd, ret;// status;
	pid_t x;
	int data_size = sizeof(buffer);
	sem_t *semaforo_1, *semaforo_2, *semaforo_3;
	buffer *shared_data;
	

	fd = shm_open("/shmtest5", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
		if (fd == -1) { perror ("shm_open"); exit (0); }
	ret= ftruncate (fd, data_size);
		if (ret == -1) { perror ("ftruncate"); exit (2); }
	shared_data = (buffer *)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
		if (shared_data == MAP_FAILED) { perror ("mmap"); exit (3); }

	if ((semaforo_1 = sem_open("semaforo_1", O_CREAT |O_EXCL, 0644, 1)) == SEM_FAILED) { perror("No sem_open()"); exit(0);}
		if ((semaforo_2 = sem_open("semaforo_2", O_CREAT |O_EXCL, 0644, 0)) == SEM_FAILED) {perror("No sem_open()");exit(0);}
			if ((semaforo_3 = sem_open("semaforo_3", O_CREAT |O_EXCL, 0644, 0)) == SEM_FAILED) {perror("No sem_open()");exit(0);}

	strcpy(shared_data->mensagens[0],"sistemas");
	strcpy(shared_data->mensagens[1],"de");
	strcpy(shared_data->mensagens[2],"computadores");
	strcpy(shared_data->mensagens[3],"a");
	strcpy(shared_data->mensagens[4],"melhor");
	strcpy(shared_data->mensagens[5],"cadeira");
			

	for(i=0;i<2;i++){ 		//cria 2 filhos
		x = fork ();
		if(x == -1){ perror("\nFork falhou"); exit(1); }	
		//pid=x;
		//if (x==0){ pid[i]=getpid(); break; }	
	

	//codigo para os processos novos (filhos)
		if(x==0){
			sem_wait(semaforo_1);								//semaforo para filho
			printf("%s\n",shared_data->mensagens[shared_data->cont]);
			shared_data->cont++;
			sem_post(semaforo_2);				 	//desbloqueia o primeiro pai

			}
		else if(x>0) {

			sem_wait(semaforo_2);				//pai fica à espera que o ultimo processo filho o deixe continuiar
			printf("%s",shared_data->mensagens[shared_data->cont]);
			shared_data->cont++;
			sem_post(semaforo_1);


		}
		
		
		
		

	}
	

	//aqui para todos
	//sem_wait(semaforo_3);
	//sem_post(semaforo_3);
		

	
	
	//************ destruir semaforos
	if(i>0) exit(0);
	r = sem_unlink("semaforo_1");
		if (r < 0) {  perror("No sem_destroy()"); exit(1); }
		
		r = sem_unlink("semaforo_2");
		if (r < 0) {  perror("No sem_destroy()"); exit(1); }
		
		r = sem_unlink("semaforo_3");
		if (r < 0) {  perror("No sem_destroy()"); exit(1); }
		
		ret = munmap (shared_data, sizeof (shared_data));
		if (-1 == ret) { perror ("munmap"); exit (1); }
		ret = shm_unlink ("/shmtest5");
		if (-1 == ret) { perror ("unlink"); exit (1); }



 return 0;

}

