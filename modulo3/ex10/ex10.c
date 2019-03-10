/*
 * Implement a program that creates a new process. One will be the producer and the other the consumer. Among
them should be created a circular buffer to store 10 integers and the necessary synchronization variables in a
shared memory area. The producer puts increasing values in the buffer that should be printed by the consumer.
Consider that 30 values are exchanged between them.
 */

#include <sys/mman.h>
#include <sys/stat.h> /* Para constantes de “modo” */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>



#define BUFFER_SIZE 10

typedef struct{
	int numero[BUFFER_SIZE];
	int flag;
}buffer;


int main() {


    int fd; // descritor da memória partilhada
    int r; // utilizado para guardar o valor retornado
    int size= sizeof(buffer);
    int i,j;
    buffer *circularMem;
    //int i;
    pid_t pid;


    pid=fork();
    if(pid<1) perror("ERRO");											//se fork der erro
    if(pid>0){ //--------------------------------------------------------codigo para o processso pai
    	fd = shm_open("/ex10", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);	// abrir criar o objeto de memória partilhada
    	if (fd < 0) { perror("No shm_open()");exit(1);}					//se der erro a criar a shared mem
   	    ftruncate(fd, size);											// ajustar o tamanho da mem. partilhada
   	 circularMem= (buffer*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mapear a mem. partilhada
    	if (circularMem == NULL) { perror("No mmap()"); exit(1); }			//sai se mmap der erro
    	circularMem->flag=0;												//flag a zero para o filho esperar

    	//variaveis de inicialização
    	for(i=0;i<3;i++){
    		while(circularMem->flag==1);
    		for(j=0;j<10;j++){
    			circularMem->numero[j]=i+j;
    		}
    		circularMem->flag=1;
    	}

	}else{ //------------------------------------------------------------filho
		fd = shm_open("/ex10", O_RDWR, S_IRWXU | S_IRWXG);				// abrir criar o objeto de memória partilhada
		ftruncate(fd, size);											// ajustar o tamanho da mem. partilhada
		circularMem= (buffer*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mapear a mem. partilhada
		if (circularMem == NULL) { perror("No mmap()"); exit(1); }			//sai se mmap der erro

    	for(i=0;i<3;i++){
    		while(circularMem->flag==0);
    		for(j=0;j<10;j++){
    			printf("%d |",circularMem->numero[j]);
    		}
    		printf("\n");
    		circularMem->flag=0;
    		}


    	}

    //destruir a shared memory

    if(pid>0){
    	while(circularMem->flag==1);
    	// o leitor( pai ) apaga a memoria partilhada do sistema
    	 r=munmap(circularMem, size); // desfaz mapeamento
    	 if (r < 0) { // verifica erro
    	        perror("No munmap()");
    	        exit(1);
    	 }
    	 else printf("mapeamento desfeito\n");

    	 r=shm_unlink("/ex10");
    	 if (r < 0) { // verifica erro
    	        perror("No unlink()");
    	        exit(1);
    	 }
    	 else printf("unlink desfeito\n");
    	}
    else{
    	//printf("%d",pid);
    }



    return 0;
}


