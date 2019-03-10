#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFERSIZE 80

int main()
{
	int fd[2], auxpid, status;
	pipe(fd);
	pid_t p;
	char enviarHello[BUFFERSIZE];//="hello world";
	char enviarGoodbye[BUFFERSIZE];//="GoodBye";
	char receber1[BUFFERSIZE];
	char receber2[BUFFERSIZE];

	p=fork();
	if(p>0){ /*codigo do pai*/
		close(fd[0]);
		/*o pai vai enviar duas mensagens para o pipe */
		printf("vou enviar duas mensagens\n");
		printf("escreva duas mensagens\n");
		scanf(" %[^\n]",enviarHello);
		write(fd[1],&enviarHello,BUFFERSIZE);
		scanf(" %[^\n]",enviarGoodbye);
		write(fd[1],&enviarGoodbye,BUFFERSIZE);
		close(fd[1]);
		do{							//esperar que o filho termine
			auxpid=waitpid(p,&status, WNOHANG);
			if (auxpid==-1)
			{
				perror("Erro em waitpid");
				exit(-1);
			}
		}while(auxpid==0);
		if(WIFEXITED(status)){
				printf("Pai %d: o filho terminou \n",getpid());
		}
	}
	else /*codigo para o filho*/
	{
		printf("tou à espera dass mensagens do pai\n");
		close(fd[1]);
		read(fd[0],&receber1,BUFFERSIZE);
		printf("o pai disse %s\n",receber1);
		read(fd[0],&receber2,BUFFERSIZE);
		printf("o pai disse %s\n",receber2);
		close(fd[0]);
	}

 return 0;
}

