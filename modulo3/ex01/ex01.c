#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

/*
 * 1. Implement a solution that sends the number and name
 * of a student between two processes not related
 * hierarchically, a writer and a reader.The writer must
 * create a shared memory area, read the data from the
 * keyboard and write them in the shared memory. The reader
 * should read the data from the shared memory and print
 * them on the screen.
 * Note: the writer must be executed before the reader
 */


/* estrutura de dados para a memoria partilhada*/
typedef struct {
	int n;
	char nome[50];
	int flag_data;
}shared_data_type;

int main(int argc, char *argv[]){
	int fd, ctrlex; //descritor da shm
	int data_size = sizeof(shared_data_type); //tamanho da shm
	char name[50];
	shared_data_type *shared_data; //apontador da shm

	pid_t p;

	fd = shm_open("/ex01", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);// cria objeto da shm
	if (fd<0){
		perror("Erro a criar objeto shm");
		exit(1);
	}

	ftruncate(fd,data_size); // ajustar o tamanho da shm

	/*mapear para o apontador a shm*/
	shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (shared_data == NULL){
		perror("Erro a mapear shm para apontador");
		exit(1);
	}

	shared_data->flag_data = 0; //colocar a flag de controlo de escrita a 0

	p = fork();
	if (p>0){ //Pai vai escrever
		printf("Digite o numero do aluno:");
		scanf("%d", &shared_data->n);
		printf("Digite o nome do aluno:");
		scanf("%[^\n]s", name);
		strcpy(name,shared_data->nome);

		shared_data->flag_data=1;

		wait(NULL);
	}else{ //filho leitor
		while(shared_data->flag_data ==0); //enquanto a flag for nula não avanca
			printf("Numero do aluno: %d\n", shared_data->n);
			printf("Nome do aluno: %s\n", shared_data->nome);

		ctrlex = munmap(shared_data, data_size); //remove mapeamento shm
		if (ctrlex<0){
			perror("Erro a remover mapeamento shm");
			exit(1);
		}

		exit(0);

	}
	ctrlex = munmap(shared_data, data_size); //remove mapeamento shm
	if (ctrlex<0){
		perror("Erro a remover mapeamento shm");
		exit(1);
	}

	ctrlex=close(fd); //Fecha escritor

	ctrlex=shm_unlink("/ex01"); //remove shm da pasta do sistema
    if (ctrlex < 0) {
        perror("Erro ao remover shm da pasta do sistema");
        exit(1);
    }

    return 0;
}
