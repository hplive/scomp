#include <sys/mman.h>
#include <sys/stat.h> /* Para constantes de “modo” */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define numElements 10

/* 2. Implement a solution that tests the speed of two transfer methods between two processes – pipes and shared
memory. Start by filling a 1.000.000 array with numbers. Start a timer and copy that array between the two
processes using shared memory. Start a timer and use a pipe to transfer the same amount of data between
processes. Test for different sizes of data

*/
typedef struct{
	int arrayParaShaMem[numElements];
}arrayMem;

void limpar(int *addr, int data_syze);

int main()
{
	
	
	int fd[2],i,arrayParaPipe[numElements];		//descritor e outras vars
	pid_t p;
	pipe(fd);
	//variaveis para  a memoria partilhada
	int fds;
	arrayMem * addr;
	int data_syze=sizeof(arrayMem);
	
	if(pipe(fd)==-1){// cria pipe
		perror("Pipe Falhou\n");
		return 1;
	}
	
	p=fork();

	if(p==0){
		close(fd[0]); //fecho da leitura
		clock_t begin = clock();
		for(i=0;i<numElements;i++){
			arrayParaPipe[i]=i+1;
			write(fd[1],&arrayParaPipe[i],sizeof(int));
		}
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("tempo para encher o array por pipe %f\n",time_spent);
		close(fd[1]); //fecho da escrita
		exit(i+1);
	}else{

		//printf("##PAI À ESPERA QUE O FILHO ENCHA O PIPE###\n");
		for(i=0;i<numElements;i++){
			if(i==0) printf("PAI A IMPRIMIR O QUE O FILHO ENVIOU\n");
				read(fd[0],&arrayParaPipe[i],sizeof(int));

		}

		close(fd[0]);
		for(i=0;i<numElements;i++)
			printf("Array[%d]=%d\n",i,arrayParaPipe[i]);
		printf("PAI IMPRIMIU TUDO QUE O FILHO ENVIOU\n");

	}

	return 0;
}
