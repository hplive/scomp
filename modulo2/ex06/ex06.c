#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
/*
 * Given two integer arrays vec1 and vec2, with 1000 elements each,
 * write a program that creates 5 child processes that communicate
 * with the parent through a pipe, to concurrently sum the two arrays.
 * Each child should calculate tmp += vec1[i] + vec2[i] on 200 elements,
 * then sending tmp to its parent through the pipe. Ensure that each child
 * computes different positions of the arrays. The parent process should
 * wait for the 5 partial sums and then calculate the final result.
 * There must be only one pipe, shared by all processes.
 */

#define numElements 1000
#define numProcess 5

int main(void){
	int vec1[numElements];
	int vec2[numElements];
	int i,j,soma, total[numElements];
	pid_t p[numProcess];
	int fd[2];
	int tamanhoporProcesso = numElements/numProcess;

	if(pipe(fd)==-1){// cria pipe
		perror("Pipe Falhou\n");
		return 1;
	}

	for(i=0;i<numElements;i++){ //povoar vectores
		vec1[i]=i;
		vec2[i]=i;
	}

	for(i=0;i<numProcess;i++){ // criar processos
			p[i]=fork();
			if(p[i]==0){
				close(fd[0]); //fecho da leitura
				for(j=i*tamanhoporProcesso;j<i*tamanhoporProcesso+tamanhoporProcesso;j++){
					soma=vec1[j]+vec2[j];
					write(fd[1],&soma,sizeof(int));
					soma=0;
				}
				close(fd[1]); //fecho da escrita
				exit(i+1);
			}else{
				for(i=0;i<numProcess;i++){
					for(j=i*tamanhoporProcesso;j<i*tamanhoporProcesso+tamanhoporProcesso;j++){
						read(fd[0],&total[j],sizeof(int));
						printf("Total[%d]=%d\n",j,total[j]);
					}
					close(fd[0]);
				}
			}
	}


	return 0;
}
