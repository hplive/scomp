#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFERSIZE 256

int main()
{
	pid_t p;
	int up[2],down[2];
	pipe(up);
	pipe(down);

	char enviar[BUFFERSIZE];
	char receber[BUFFERSIZE];


	p=fork();
	if(p==0){ /*codigo do cliente ( filho)*/
									//como vai escrever fecha o descritor de leitura
		printf("escreva uma mensagens\n");
		scanf(" %[^\n]",enviar);
		write(up[1],&enviar,BUFFERSIZE);		//envia pro tubo pelo desc escrita
		close(up[1]);							//fecha desc. de escrita
		read(down[0],&receber, BUFFERSIZE);
		close(down[0]);
		printf("%s\n",receber);
	}
	else /*codigo do servidor (pai)*/
	{
		read(up[0],&receber,BUFFERSIZE);		//ler o conteudo do pipe que o cliente enviou
		int i=0;
		for(i=0;i<BUFFERSIZE;i++){
			if(receber[i]<='z' && receber[i]>='a' )
				receber[i]=toupper(receber[i]);		//capitalizar
			else receber[i]=tolower(receber[i]);    //minimizar

		}
		close(up[0]);							//fechar o descritor de leitura enviado do cliente
		write(down[1],&receber,BUFFERSIZE);		//enviar texto convertido
		close(down[1]);							//fechar o descritor de escrita
	}

 return 0;
}


