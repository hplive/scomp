/*
Implement a program to multiply two matrices. Assumptions:
 The size of each matrix is 5x5.
 Create two threads to filled two matrices;
 The calculation should use 5 threads to solve the problem;
 The Main thread must wait for all threads to calculate and should print the calculated matrix.

*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>
#include <inttypes.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <semaphore.h>



typedef struct{
	int array[5][5];
} data_type;

typedef struct{
	int linhas[5];
	int colunas[5];
	int linha_res[5];
} multiplicacao_type;



void * escreve(void *arg)
{
	data_type *var = (data_type *) arg;
	//inicialização do array
		 for ( int r=0; r<5; r++ )
		 {
			 printf("\n"); for ( int c=0; c<5; c++ ) {
				 var->array[r][c]= r*c+1;
				 printf("%d ", var->array[r][c]);
			 }
		 }
	pthread_exit(NULL);
}

void * multiplica(void *arg)
{
	multiplicacao_type *var = (multiplicacao_type *) arg;
	//inicialização do array
	int i;
	for (i=0; i<5; i++ ){
		var->linha_res[i]=var->linhas[i]*var->colunas[i];
	}
	pthread_exit(NULL);
}
int main()
{
	 int fd, r; // descritor da memória partilhada
	 int size= sizeof(multiplicacao_type);
	 multiplicacao_type *memoria;


	int array0[5][5]; //array
	int array1[5][5];

	data_type array_resultado;

	//initialize
	pthread_t threads[2];
	pthread_t thread[5];
	int s1,s2;
	s1 = pthread_create(&threads[0], NULL, escreve, (void *) &array0);
	s2 = pthread_create(&threads[1], NULL, escreve, (void *) &array1);
	if (s1!=0 && s2!=0) {
		perror("Erro ao criar o thread");
		exit(-1);
	}


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
   	 memoria= (multiplicacao_type*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mapear a mem. partilhada
    	if (memoria == NULL) { perror("No mmap()"); exit(1); }			//sai se mmap der erro
	//****************************************************************************************************************

	int i,j;
		for(i=0;i<5;i++){
			//coluna
			for(j=0;j<5;j++){
				memoria->colunas[j]=array1[i][j];
			}
			int k;
			for(k=0;k<5;k++) memoria->linhas[k]=array0[i][k];
			s1 = pthread_create(&thread[i], NULL, multiplica, (void *) &memoria);
			if (s1!=0) {
				perror("Erro ao criar o thread");
				exit(-1);
			}
			for(k=0;k<5;k++)
			array_resultado.array[i][k]=memoria->linha_res[k];

		}

	sleep(1);
	printf("\n---------------------------\n");
	for(i=0;i<5;i++){
		for(j=0;j<5;j++)
			printf("%d ",array_resultado.array[i][j]);
		printf("\n");
	}


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

pthread_exit(NULL);
}
