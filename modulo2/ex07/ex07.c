/*
 * Given two integer arrays vec1 and vec2, with 1000 elements each, write a program that
 * creates 5 child processes to concurrently sum the two arrays. Each child should calculate
 * vec1[i] + vec2[i] on 200 elements, sending all those values to its parent. Ensure that each
 * child computes different positions of the arrays. The parent process should wait for all the
 * 1000 values and then store them in a result array, in the correct position. Use 5 pipes,
 * one for each child.
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFERSIZE 1000

int main()
{
	pid_t p;
	int vec1[BUFFERSIZE],vec2[BUFFERSIZE], result[BUFFERSIZE/5];
	int pipeline[5][2];

	/*inicializar o vector*/
	int i;
	for (i = 0; i < BUFFERSIZE; i++){
		vec1[i] = 1;//rand () % 2;
		vec2[i] = 1;//rand () % 2;
	}



	//criar os processos filhos
	for(i=0;i<5;i++){
		pipe(pipeline[i]);
		int soma=0;
		p=fork();

		if(p==0){ //codigo para o processo filho
			printf("HELLO WORLD\n");
			int j, inicioContagem, fimContagem;
			inicioContagem=200*i;
			fimContagem=inicioContagem+200;

			for(j=inicioContagem;j<fimContagem;j++){
				soma+=vec1[j]+vec2[j];
			}


			write(pipeline[i][1],&soma,sizeof(int));		//envia pro tubo pelo desc escrita
			close(pipeline[i][1]);							//fecha desc. de escrita
			break;

		}
		else if( p>0){//codigo para o pai
			read(pipeline[i][0],&soma,sizeof(int));		//ler o conteudo do pipe que o cliente enviou
			result[i]=soma;
			close(pipeline[i][0]);						//fechar o descritor de leitura enviado do cliente


		}


	}

	if(p>0){
		printf("RESULTADO [0]=%d\n",result[0]);
		printf("RESULTADO [1]=%d\n",result[1]);
		printf("RESULTADO [2]=%d\n",result[2]);
		printf("RESULTADO [3]=%d\n",result[3]);
		printf("RESULTADO [4]=%d\n",result[4]);
	}



 return 0;
}


