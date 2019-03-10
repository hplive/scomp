/*.
 Implement a program that allows the exchange of data concerning a student between two processes (number,
name and grades of a set of classes). The data to be exchanged are represented in the following struct aluno.
1 #define STR_SIZE 50
2 #define NR_DISC 10
3 struct aluno{
4 int numero;
5 char nome[STR_SIZE];
6 int disciplinas[NR_DISC];
7 };
The parent process should:
• Create a shared memory area for data exchange. Check the need to add one or more variables to synchronize
the writing and reading of data operations;
• Create a new process;
• Fill the shared memory area in accordance with user-entered information;
• Wait until the child process ends.
• Eliminate the shared memory area.
The child process should:
• Wait for the student data;
• Calculates the highest, the lowest and the average grade;
• Print the information on the screen
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


#define STR_SIZE 50
#define NR_DISC 3

typedef struct{
	int numero;
	char nome[STR_SIZE];
	int disciplinas[NR_DISC];
	int flag;
}aluno;


int main() {


    int fd; // descritor da memória partilhada
    int r; // utilizado para guardar o valor retornado
    int size= sizeof(aluno);
    int i,j;
    aluno *student;
    //int i;
    pid_t pid;


    pid=fork();
    if(pid<1) perror("ERRO");											//se fork der erro
    if(pid>0){ //--------------------------------------------------------codigo para o processso pai
    	fd = shm_open("/ex08", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);	// abrir criar o objeto de memória partilhada
    	if (fd < 0) { perror("No shm_open()");exit(1);}					//se der erro a criar a shared mem
   	    ftruncate(fd, size);											// ajustar o tamanho da mem. partilhada
    	student= (aluno*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mapear a mem. partilhada
    	if (student == NULL) { perror("No mmap()"); exit(1); }			//sai se mmap der erro
    	student->flag=0;												//flag a zero para o filho esperar

    	//variaveis de inicialização
    	for(i=0;i<3;i++){
    		while(student->flag==1);

    		switch(i){
    			case 0:{ student->numero=1000; for(j=0;j<NR_DISC;j++)student->disciplinas[j]=i+10+j;strcpy(student->nome,"helder"); student->flag=1; break; }
    			case 1:{ student->numero=1001; for(j=0;j<NR_DISC;j++)student->disciplinas[j]=i+11+j;strcpy(student->nome,"manuel"); student->flag=1; break;}
    			case 2:{ student->numero=1002; for(j=0;j<NR_DISC;j++)student->disciplinas[j]=i+12+j;strcpy(student->nome,"pereira"); student->flag=1; break;}
    			default: break;
    		}


    	}

	}else{ //------------------------------------------------------------filho
		fd = shm_open("/ex08", O_RDWR, S_IRWXU | S_IRWXG);				// abrir criar o objeto de memória partilhada
		ftruncate(fd, size);											// ajustar o tamanho da mem. partilhada
		student= (aluno*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mapear a mem. partilhada
		if (student == NULL) { perror("No mmap()"); exit(1); }			//sai se mmap der erro

    	for(i=0;i<3;i++){
    		while(student->flag==0);
    		printf("numero %d\n",student->numero);
    		printf("nome %s\n",student->nome);
    		printf("DISCIPLINAS : ");
    		for(j=0;j<NR_DISC;j++) printf("%d ",student->disciplinas[j]); printf("\n");
    		student->flag=0;

    		}


    	}

    //destruir a shared memory

    if(pid>0){
    	while(student->flag==1);
    	// o leitor( pai ) apaga a memoria partilhada do sistema
    	 r=munmap(student, size); // desfaz mapeamento
    	 if (r < 0) { // verifica erro
    	        perror("No munmap()");
    	        exit(1);
    	 }
    	 else printf("mapeamento desfeito\n");

    	 r=shm_unlink("/ex08");
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


