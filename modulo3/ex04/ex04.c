/*. Implement a program that creates a shared memory area to store an integer, initializes this value to 100, and
creates a new process. Parent and child must perform the following operations 1.000.000 times:
• Increase the value;
• Decrease the value;
• Only write the value on the screen at the end.
Review the results. Will these results always be correct?
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

#define operations 10000000


typedef struct{
	int num;
	int flag;
}areaStore;


int main() {
    int fd; // descritor da memória partilhada
    int r; // utilizado para guardar o valor retornado
    int size= sizeof(areaStore);
    areaStore *numero;
    int i;
    pid_t pid;


    // abrir/criar o objeto de memória partilhada
     fd = shm_open("/ex04", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    if (fd < 0) {
        perror("No shm_open()");
        exit(1);
    }

     // ajustar o tamanho da mem. partilhada
    ftruncate(fd, size);

    // mapear a mem. partilhada
    numero= (areaStore*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    numero->num=100; //inicializar num


    if (numero == NULL) {		//sai se mmap der erro
        perror("No mmap()");
       exit(1);
    }

     pid=fork();

    if(pid>0){  //codigo para o processso pai

		for(i=0;i<operations;i++){ //somar n vezes
			numero->num++;
		}
		//printf("%d | ",numero->num);

		for(i=operations;i>0;i--){ //subtrair n vezes
			numero->num--;
		}
		//printf("%d | ",numero->num);
		printf("PAI:%d\n",numero->num);  //imprimir o numero somado pelo pai
		numero->flag=1;					//flag para o filho esperar que o pai termine

	}else{ //filho
		while(numero->flag==0);  //se nao tiver esta flag o resultado varia.
		for(i=0;i<operations;i++){
			numero->num++;
		}
		//printf("%d | ",numero->num);
		for(i=operations;i>0;i--){
				numero->num--;
		}
		//printf("%d | ",numero->num);
		printf("FILHO:%d\n",numero->num);
		numero->flag=0;

	}

    //destruir a shared memory

    if(pid>0){
    	while(numero->flag==1);
    	// o leitor( pai ) apaga a memoria partilhada do sistema
    	 r=munmap(numero, size); // desfaz mapeamento
    	 if (r < 0) { // verifica erro
    	        perror("No munmap()");
    	        exit(1);
    	 }
    	 else printf("mapeamento desfeito\n");

    	 r=shm_unlink("/ex04");
    	 if (r < 0) { // verifica erro
    	        perror("No unlink()");
    	        exit(1);
    	 }
    	 else printf("unlink desfeito\n");
    	}
    else{
    	//printf("%d",pid);
    }

    if(pid>0) //so para um imprimir a resposta seguinte
    printf("#####################################\nSE NAO TIVER O WHILE DA LINHA 75 E A FLAG O RESULTADO VARIA\n##########################################\n");




    return 0;
}


