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
 * 3. Implement a solution that allows you to share an array
 * of 10 integers between two processes not related hierarchically,
 *  a writer and a reader. The writer must create a shared memory
 *  area, generate 10 random numbers between 1 and 20 and write
 *  them in the shared memory. The reader should read the 10 values,
 *  calculate and print the average.
 *  Note: the writer must be executed before the reader.
 */


/* estrutura de dados para a memoria partilhada*/
#define N_VALUES 10
#define RANGE 20

#define SHMEX "/EX03"


typedef struct{
	int numbers[N_VALUES];
} shared_data_type;


int main(int argc, char *argv[]){

	time_t t;

	/* Iniciar o gerador de numeros*/
	srand((unsigned) time(&t));

	int fd, i, error, status, soma = 0, media= 0, ctrlex;
	int data_size = sizeof(shared_data_type); //tamanho da shm
	int vec[N_VALUES];

	shared_data_type *shared_data; //apontador da shm

	pid_t p[N_VALUES];

	fd = shm_open(SHMEX, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); // cria objeto da shm
	if (fd<0){
		perror("Erro a criar objeto shm");
		exit(1);
	}

	error = ftruncate(fd,data_size); // ajustar o tamanho da shm
	if(error == -1){
		perror("Falha ao ajustar tamanho SHM");
		exit(2);
	}

	/*mapear para o apontador a shm*/
	shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (shared_data == NULL){
		perror("Erro a mapear shm para apontador");
		exit(1);
	}

	for(i=0; i<N_VALUES; i++){
		vec[i]=generateNumber(RANGE);
	}

	/* Cria processos */
	for(i=0;i<N_VALUES;i++){
		p[i] = fork();

		if(p[i] == 0){/* Filho */
			shared_data->numbers[i] = vec[i]; // alocar numero à SHM
			printf("Posição [%d] = %d\n", i+1, shared_data->numbers[i]);
			exit(i+1);
		}
	}
	for(i=0;i<N_VALUES;i++){
			waitpid(p[i],&status,0);
	}

	for(i=0;i<N_VALUES;i++){
		soma+= shared_data->numbers[i];/* search for the greatest */
	}
	media = soma / N_VALUES;

	printf("A media dos numeros alocados é: %d.\n", media);

	ctrlex = munmap(shared_data, data_size); //remove mapeamento shm
	if (ctrlex<0){
		perror("Erro a remover mapeamento shm");
		exit(1);
	}

	ctrlex=close(fd); //Fecha escritor

	ctrlex=shm_unlink(SHMEX); //remove shm da pasta do sistema
    if (ctrlex < 0) {
        perror("Erro ao remover shm da pasta do sistema");
        exit(1);
    }

    return 0;
}
