#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
/*
 * Write a program that creates a child process
 * and establishes with it a communication channel through a pipe.
 * The parent process reads an integer and a string from stdin and sends
 * both through the pipe. The child process should read those values
 * from the pipe and print them.
 *
 * a. Solve the exercise by first sending an integer and then a
 * string in two separate write operations;
*/


int main(void){
	pid_t p;
	int fd[2];
	int num;
	char string1[80];
	/* cria pipe */
	if(pipe(fd)==-1){
		perror("Pipe Falhou\n");
		return 1;
	}
	p = fork();
	if(p==-1){
			perror("Fork failed."); //se valor devolvido pelo fork for -1 erro na criação do clone processo
			exit(-1);
	}
	if(p>0){ //Pai vai escrever

		close(fd[0]); //fecha canal de leitura
		printf("O Pai escreve no Pipe:\n");
		printf("Por digite um número: ");
		scanf("%d",&num);
		printf("Por digite uma string: ");
		scanf("%s",string1);
		write(fd[1],&num,sizeof(int));
		write(fd[1],string1,strlen(string1)+1);
		close(fd[1]); //fecha canal de escrita
		wait(NULL);
	}else{ // filho vai ler
		close(fd[1]); // fecha canal de escrita
		read(fd[0],&num,sizeof(int)); //lê num
		printf("Número: %d\n",num);
		read(fd[0],string1,80);// lê a string
		printf("String: %s\n",string1);
		close(fd[0]); // fecha canal de leitura
		printf("Valores lidos pelo filho no Pipe:\n");
		exit(0);
	}

	return 0;
}
