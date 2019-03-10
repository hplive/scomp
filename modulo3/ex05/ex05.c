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
 * 5. Implement a program to determine the biggest element of an array in a
 * parallel/concurrent environment. The parent process should:
 * - Create an array of 1000 integers, initializing it with random values between 0 and 1000;
 * - Create a shared memory area to store an array of 10 integers, to be used
 * for the local maximum determined by each child process;
 * - Create 10 new processes;
 * - Wait until the 10 child processes finish the search for the local maximum;
 * - Determine the global maximum;
 * - Eliminate the shared memory area.
 * Each child process should:
 * - Calculate the largest element in the 100 positions;
 * - Write the value found in the position corresponding to the
 * order number (0-9) in the array of local maximum.
 */


/* estrutura de dados para a memoria partilhada*/
#define N_MAX 10
#define RANGE 1000

#define SHMEX "/EX05"


typedef struct{
	int maxNumbers[N_MAX];
} shared_data_type;


int main(int argc, char *argv[]){

	time_t t;

	/* Iniciar o gerador de numeros*/
	srand((unsigned) time(&t));

	int fd, i, f, error, status, maximo=0, ctrlex;
	int data_size = sizeof(shared_data_type); //tamanho da shm
	int vec[RANGE];
	int tamanhoProcura = RANGE/N_MAX;


	shared_data_type *shared_data; //apontador da shm

	pid_t p[N_MAX];

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

	for(i=0; i<RANGE; i++){
		vec[i]=generateNumber(RANGE);
		//printf("posicao %d numero gerado: %d\n", i+1, vec[i]);
	}

	/* Cria processos */
	for(i=0;i<N_MAX;i++){
		p[i] = fork();

		if(p[i] == 0){/* Filho */
			for(f=i*tamanhoProcura;f<(i+1)*tamanhoProcura; f++){
				if(maximo<vec[f]){ //procurar o maximo
					maximo = vec[f]; //alocar o maximo
				}
			}
			shared_data->maxNumbers[i] = maximo; // alocar numero maximo à SHM
			printf("Máximo valor do processo %d = %d\n", i+1, maximo);
			exit(i+1);
		}
	}
	for(i=0;i<N_MAX;i++){
			waitpid(p[i],&status,0);
	}

	for(i=0;i<N_MAX;i++){
		if(maximo<shared_data->maxNumbers[i]){
			maximo = shared_data->maxNumbers[i];
		}
	}

	printf("O máximo global é: %d.\n", maximo);

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
