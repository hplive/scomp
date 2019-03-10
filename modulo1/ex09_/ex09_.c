#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

/*
 * Write a program that creates 10 child processes. Each child process should
 * print 100 numbers according to its creation order. That is the first child
 * process created writes numbers 1..100, the second child process 101..200,
the third 201..300, and so on. The parent process should wait for all child
processes to finish.
a) Is the output sorted? Can you guarantee it will be always sorted? Why?
*/

int main(void){
	int i,j,k=1,n=10;
	pid_t p;
	for(i=0;i<n;i++){
		p=fork();
		if(p==-1){
				perror("Fork failed."); //se valor devolvido pelo fork for -1 erro na criação do clone processo
				exit(-1);
			}else if(p==0){			 //inicia código filho
				printf("Iniciou o filho %d\n",i + 1);
				for(j=0;j<100;j++){
					printf("Nº %d \n", (i*100)+j);
				}
				exit(0);
			}
	}
	while (wait(NULL) > 0){
	    printf("O filho %d terminou\n", k++);
	}

	printf("Pai: Todos os filhos terminaram! \n");
	return 0;
}
