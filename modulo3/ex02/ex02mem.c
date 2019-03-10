
#include <sys/mman.h>
#include <sys/stat.h> /* Para constantes de “modo” */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define numElements 10

/* 2. Implement a solution that tests the speed of two transfer methods between two processes – pipes and shared
memory. Start by filling a 1.000.000 array with numbers. Start a timer and copy that array between the two
processes using shared memory. Start a timer and use a pipe to transfer the same amount of data between
processes. Test for different sizes of data

*/

typedef struct{
	int arrayParaShaMem[numElements];
}arrayMem;


int main() {
    int fd; // descritor da memória partilhada
    int r; // utilizado para guardar o valor retornado
    int size= sizeof(arrayMem);
    arrayMem *numero;
    int i;
    pid_t pid;


    // abrir/criar o objeto de memória partilhada
     fd = shm_open("/ex02", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    if (fd < 0) {
        perror("No shm_open()");
        exit(1);
    }

     // ajustar o tamanho da mem. partilhada
    ftruncate(fd, size);

    // mapear a mem. partilhada
    numero= (arrayMem*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    //*numero=100;

    if (numero == NULL) {
        perror("No mmap()");
       exit(1);
    }

     pid=fork();

    if(pid>0){

		for(i=0;i<numElements;i++){
			numero->arrayParaShaMem[i]=i;
		}

		for(i=0;i<numElements;i++){
			printf("Pai:%d\n", numero->arrayParaShaMem[i]);
	}

	}else{

		for(i=0;i<numElements;i++){
			printf("Filho:%d\n", numero->arrayParaShaMem[i]);

	}

    /*
     * a parte abaixo nao e' pedida no exercicio
     */



    if(pid>0){

    	// o leitor( pai ) apaga a memoria partilhada do sistema
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


    return 0;
}

