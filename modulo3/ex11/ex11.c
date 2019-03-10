/*
 * Solve again question 10, but now the synchronization between the processes should be supported by pipes,
instead of shared variables
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

}buffer;


int main() {


    int fd; // descritor da memória partilhada
    int r; // utilizado para guardar o valor retornado
    int size= sizeof(buffer);
    int i,j;
    buffer *circularMem;
    //int i;
    pid_t pid;
    int status=0; //variavel de control entre os dois pipes | substitui a antiga flag no exercicio anterior
    int auxpid;  //variavel para esperar pelo pid do filho


    //sincronização por pipes
    int pipefd1[3][2];		//descritor pipe, pai escreve, filho lê
    int pipefd2[3][2];		//descritor pipe, filho escreve pai lê

    for(i=0;i<3;i++){		//criar pipes e verificar se ocurreu algum erro

    	if(pipe(pipefd1[i])==-1) {// cria pipes 1
    		perror("Pipe1 falhou\n");
    		return 1;
    	}
    	if(pipe(pipefd2[i])==-1) {// cria pipes 2
    	    		perror("Pipe2 falhou\n");
    	    		return 1;
    	    	}
    }


    pid=fork();
    if(pid<1) perror("ERRO");											//se fork der erro
    if(pid>0){ //--------------------------------------------------------codigo para o processso pai
    	fd = shm_open("/ex11", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);	// abrir criar o objeto de memória partilhada
    	if (fd < 0) { perror("No shm_open()");exit(1);}					//se der erro a criar a shared mem
   	    ftruncate(fd, size);											// ajustar o tamanho da mem. partilhada
   	 circularMem= (buffer*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mapear a mem. partilhada
    	if (circularMem == NULL) { perror("No mmap()"); exit(1); }			//sai se mmap der erro


    	//variaveis de inicialização
    	for(i=0;i<3;i++){
    		status=1;
    		write(pipefd1[i][1],&status,sizeof(int));		//envia status 1 , que significa vai escrever
    		for(j=0;j<10;j++){
    			circularMem->numero[j]=i+j;
    		}
    		close(pipefd1[i][1]);							//fecha desc. de escrita

    		do{
    		    read(pipefd2[i][0],&status,sizeof(int));	//ler o controlo do status, se for zero o filho já leu
    		}while(status!=0);
    		close(pipefd2[i][0]);							//fechar o pipe que diz que a leitura ja foi executada
    	}

	}else{ //------------------------------------------------------------filho
		fd = shm_open("/ex11", O_RDWR, S_IRWXU | S_IRWXG);				// abrir criar o objeto de memória partilhada
		ftruncate(fd, size);											// ajustar o tamanho da mem. partilhada
		circularMem= (buffer*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mapear a mem. partilhada
		if (circularMem == NULL) { perror("No mmap()"); exit(1); }			//sai se mmap der erro

    	for(i=0;i<3;i++){
    		do{
    			read(pipefd1[i][0],&status,sizeof(int));		//ler se o pai já escreveu na shared memory
    		}while(status!=1);
    		for(j=0;j<10;j++){
    			printf("%d |",circularMem->numero[j]);
    		}
    		printf("\n");
    		close(pipefd1[i][0]);						//fechar o descritor de leitura

    		//escrever no pipe 2
    		status=0;
    		write(pipefd2[i][1],&status,sizeof(int));		//enviar info para o pai que o filho já leu e imprimiu, desbloqueia o pai para poder alterar a shared memory
    		close(pipefd2[i][1]);						//fecha o pipe2 de escrita do filho


    		}


    	}

    //destruir a shared memory, só o pai destroi depois de esperar pelo filho, para ter a certeza que imprimiu tudo

    if(pid>0){
    	do{
    		auxpid=waitpid(pid, &status, WNOHANG);
    	}while(auxpid==0);
    	//while(circularMem->flag==1);
    	// o leitor( pai ) apaga a memoria partilhada do sistema
    	 r=munmap(circularMem, size); // desfaz mapeamento
    	 if (r < 0) { // verifica erro
    	        perror("No munmap()");
    	        exit(1);
    	 }
    	 else printf("mapeamento desfeito\n");

    	 r=shm_unlink("/ex11");
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


