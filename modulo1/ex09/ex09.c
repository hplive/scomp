#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

/*
 * Write a program that creates 10 child processes. Each child process should
 * print 100 numbers according to its creation order. That is the first child
 * process created writes numbers 1..100, the second child process 101..200,
the third 201..300, and so on. The parent process should wait for all child
processes to finish.
a) Is the output sorted? Can you guarantee it will be always sorted? Why?
*/

int main(void){

	pid_t pid[10];
	int aux;
	int status;
	/*ontador de numeros dos filhos*/
	int count=1;

	int i=0;
	for(i=0;i<10;i++){
		pid[i]=fork();
		if (pid[i]<0)    /*se o fork nao for criado e der erro devolve -1 */
		{
			perror("Erro ao cria o processo\n");
			exit(-1);
		}
		if(pid[i]==0){
			int j=0;
			int limite=100*count;
			printf("eu sou o %dº filho\n",i+1);
			for(j=limite-100;j<limite;j++){
				printf("%d\n",j);
			}
			exit(count+1);
		}
		/*codigo do pai*/
		if(pid[i]>0){
			printf("eu sou o pai e vou esperar pelo filho %d\n",pid[i]);
			do{  /*à espera que o filho termine*/
				aux=waitpid(pid[i], &status, WNOHANG);
				if (aux==-1)
					 {
						 perror("Erro em waitpid");
						 exit(-1);
					 }

			}while(aux==0);
		} /*fim do if para o pai*/


		if (WIFEXITED(status))
				{
					count=WEXITSTATUS(status);
					printf("Pai: o %dº filho retornou o valor do count:%d\n",i+1,WEXITSTATUS(status));
				}

		}/*fim do for*/
	return 0;
}
