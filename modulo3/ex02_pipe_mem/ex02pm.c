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
	int flag;
}arrayMem;

void enviar_por_pipe(int *fd, int i, int *arrayParaPipe, pid_t p);
void enviar_por_shared_memory(int fds, arrayMem *numero, int r, int pid, int i, int size);



int main()
{

	//variavei para pipe

	int fd[2],i,arrayParaPipe[numElements];		//descritor e outras vars
	pid_t p;
	pipe(fd);

	//variaveis para shared mem
	int fds; // descritor da memória partilhada
	int r; // utilizado para guardar o valor retornado
	int size= sizeof(arrayMem);
	arrayMem *numero;

	// cria pipe
	if(pipe(fd)==-1){
		perror("Pipe Falhou\n");
		return 1;
	}

	p=fork();
	enviar_por_pipe(fd, i, arrayParaPipe, p);
	enviar_por_shared_memory(fds, numero, r, p, i, size); //testar por shared memory


	return 0;
}
void enviar_por_pipe(int *fd, int i, int *arrayParaPipe, pid_t p){
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
		//exit(i+1);

	}else{	  //velhote
		//printf("##PAI À ESPERA QUE O FILHO ENCHA O PIPE###\n");
		for(i=0;i<numElements;i++){
			if(i==0) printf("PAI A IMPRIMIR O QUE O FILHO ENVIOU POR PIPE\n");
				read(fd[0],&arrayParaPipe[i],sizeof(int));

		}

		close(fd[0]);
		for(i=0;i<numElements;i++)
		printf("Array[%d]=%d\n",i,arrayParaPipe[i]);
		printf("PAI IMPRIMIU TUDO QUE O FILHO ENVIOU POR PIPE\n");



	}

}

void enviar_por_shared_memory(int fds, arrayMem *numero, int r, int pid, int i, int size){
	 // abrir/criar o objeto de memória partilhada
	    fds = shm_open("/ex02", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
	    if (fds < 0) {
	        perror("No shm_open()");
	        exit(1);
	    }

	     // ajustar o tamanho da mem. partilhada
	    ftruncate(fds, size);

	    // mapear a mem. partilhada
	    numero= (arrayMem*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fds, 0);


	    if (numero == NULL) {
	        perror("No mmap()");
	       exit(1);
	    }

	    if(pid>0){

	    	printf("PAI A GUARDAR NA SHARED MEMORY\n");
	       	clock_t begin = clock();
	   		for(i=0;i<numElements;i++){
	   			numero->arrayParaShaMem[i]=i;
	   		}
	   		clock_t end = clock();
	   		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	   		printf("tempo para encher o array por memoria partilhada %f\n",time_spent);

	   		//for(i=0;i<numElements;i++) printf("Pai:%d\n", numero->arrayParaShaMem[i]);


	   	}else{

	   		printf("FILHO A IMPRIMIR O QUE ESTÁ NA SHARED MEMORY\n");
	   		for(i=0;i<numElements;i++){
	   			printf("Filho:%d\n", numero->arrayParaShaMem[i]);

	   	}

	    if(pid>0){

	    // o pai apaga a memoria partilhada do sistema
	       	 r=munmap(numero, size); // desfaz mapeamento
	       	 if (r < 0) { // verifica erro
	       	        perror("No munmap()");
	       	        exit(1);
	       	 }
	       	 else printf("mapeamento desfeito\n");

	       	 r=shm_unlink("/ex02");
	       	 if (r < 0) { // verifica erro
	       	        perror("No unlink()");
	       	        exit(1);
	       	 }
	       	 else printf("unlink desfeito\n");
	       	}
	       else{
	       	printf("%d",pid);
	       }


	       }

}
