/*
 * Implement a program that creates 8 new processes.
• Each process reads 200 Numbers from a text file (“Numbers.txt”) and write those numbers to the file “Output.txt”;
• At any time only one child should be able to read/write into the files (one process in each of the files);
• At the end, the father process should print the content of the file “Output.txt”.

 */



#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>

#define NPROCESSOS 8

int main(int argc, char *argv[]) {
	int idp, i, status, tamanho;
	sem_t *semaforo;
	pid_t pid;
	char ch;
	
	//*******GERAR NUMEROS NO FICHEIRO numbers.txt***************
	FILE *fileopen;							//ler ficheiro
	fileopen = fopen("numbers.txt","w");
	if( fileopen == NULL ){ 				//abrir ficheiro numbers. "a" Acrescenta a um arquivo. Operações de gravação, anexar dados no final do arquivo. O arquivo é criado se não existir.
		printf("Erro ao abrir ficheiro\n");				//mensagem caso dê NULL
	exit(1);								//terminar deviado a erro
	}
	// gerar os  numeros
	tamanho=200*NPROCESSOS;
	for (i=0;i<tamanho;i++){
		fprintf(fileopen, "%d ", i);	//escrever no ficheiro
	}
	 fclose(fileopen);						//fechar ficheiro
	 //**********************************************************
	 fclose(fopen("output.txt","w"));

	// criar o semáforo com valor = 1
	if((semaforo=sem_open("semaforo1", O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
		perror("No sem_open	()");
		exit(1);
	}	
	 //criar X novos processos
	for(idp=0;idp<NPROCESSOS;idp++){
		pid=fork();
		if(pid ==-1){	//verificação do erro do fork
			perror("Fork falhou");
		}
		//codigo para os processos novos (filhos)
		if(pid==0){
			sem_wait(semaforo);						//decrementa uma unidade no semaforo --neste caso foi inicializado a 1
			FILE *fileOutput;
			fileOutput = fopen("output.txt","a");	//abre ou cria ou ficheiro para "appending "
			FILE *fileopen;
			fileopen=fopen("numbers.txt","r");
			int limit_count=200*(1+idp), start_count=200*(idp), normal_count=start_count;;							//contador dos 200 numeros que cada um tem que ler

			fprintf(fileOutput,"-----------------filho %d --------------- ",idp);

			while (!feof (fileopen)) {
				fscanf (fileopen, "%d", &i);
				printf("%d - ",idp);
				if(normal_count>=start_count){
				     fprintf(fileOutput,"%d ",i);
				}
				if(normal_count>=limit_count) break;
				normal_count++;
			    }

			fclose(fileOutput);
			fclose(fileopen);

			sem_post(semaforo);						//incrementar semaforo para que outro processo possa continuar
			exit(0);								//fim de processo filho i
	}

	//pai 
	//esperar pelos filho guardar todos os pid's
	waitpid(pid,&status,0);
		if(!WIFEXITED(status)){
			printf("Erro!\n");
		}


	

	}

		
	FILE *fileOutput;
	fileOutput = fopen("output.txt","r");

 	while((ch=fgetc(fileOutput))!=EOF){
		printf("%c",ch);
	}
	fclose(fileOutput);
	
	if((sem_unlink("semaforo1")<0)){
		perror("No sem_destroy");
		exit(1);
	}

	return 0;
	
	
	
}


