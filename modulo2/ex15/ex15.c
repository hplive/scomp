#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#define BUFFER_SIZE 80

int main()
{

	char buffer[BUFFER_SIZE];
	int fd[2],i,n;								//descritor e outras vars
	pid_t p;
	pipe(fd);


	p=fork();

	if(p==0){
		printf("processo filho\n");
		close(fd[0]);										//fechar desc de leitura
		dup2(fd[1],1);										//redireccionar stdout para descritor de escrita
		close(fd[1]);										//fechar estremidade de escrita do pipe
		execlp("./factorial","./factorial","5",(char*)NULL);//execlp com argumento 5
		perror("exec error");
		exit(1);
	}
	else{
		printf("Processo pai...\n");
		close(fd[1]);								//fechar estremidade de escrita
		while((n = read(fd[0],buffer,80))!=0){		//ler pipe até que esteja razio o eof seja atingido
		buffer[n-1] = 0;
		printf("Pai leu: %s\n",buffer);
		}
		close(fd[0]);								//fechar estrimidade de leitura
		wait(NULL);
	}


	return 0;

}
