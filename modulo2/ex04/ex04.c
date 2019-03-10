#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
/*
 * Write a program that creates a child process and establishes with
 *  it a communicating channel through a pipe. The parent process should
 *  send the contents of a text file to its child through the pipe. The child
 *  should print all the received data.
 *  The parent must wait for its child to end.
 */

#define txt_size 500

int main(void){
	pid_t p;
	int fd[2];
	char filetxt[txt_size];
	char* fileName = "ex04.txt";
	FILE * ficheiro;
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
	if(p>0){ //Pai vai ler ficheiro e escrever no pipe
		ficheiro = fopen(fileName,"r"); // abre o ficheiro em modo "r" leitura
		close(fd[0]); //fecha canal de leitura
		if (ficheiro) {
			while(fgets(filetxt, txt_size, (FILE*)ficheiro) != NULL){
				write(fd[1], &filetxt, sizeof(filetxt));
			}
		}
		fclose(ficheiro);
		close(fd[1]); //fecha canal de escrita
		wait(NULL);
	}else{ // filho vai ler
		close(fd[1]); // fecha canal de escrita
		while(read(fd[0], &filetxt, sizeof(filetxt)) != 0){
			printf("%s", filetxt);
		}
		close(fd[0]); // fecha canal de leitura
		printf("\nValores lidos pelo filho no Pipe!\n");
		exit(0);
	}
	return 0;
}
