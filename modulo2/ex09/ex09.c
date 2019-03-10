/*
 * Given two integer arrays vec1 and vec2, with 1000 elements each, write a program that
 * creates 5 child processes to concurrently sum the two arrays. Each child should calculate
 * vec1[i] + vec2[i] on 200 elements, sending all those values to its parent. Ensure that each
 * child computes different positions of the arrays. The parent process should wait for all the
 * 1000 values and then store them in a result array, in the correct position. Use 5 pipes,
 * one for each child.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFERSIZE 50000;

typedef struct{
	 int customer_code;
	 int product_code;
	 int quantity;
}vendas;

int main()
{
	int resultadosEncontrados[500];
	int pipeline[2];
	int ocurrencias=0;
	int status;
	pid_t p[10];
	vendas vec[500];  //array para guardar as vendas >=20
	vendas venda;


	/*inicializar os registos*/
	int i;
	for (i = 0; i < 500; i++){
		venda.customer_code = rand () % 500;
		venda.product_code = rand () % 50;
		venda.quantity = rand () % 30;
		vec[i]=venda;

	}
	if(pipe(pipeline)==-1){
		perror("Erro a criar o pipe\n");
		return 1;
	}

	//criar os processos filhos
	int k;
	for(k=0;k<10;k++){
		p[k]=fork();
		if(p[k]==-1) {perror("Erro a criar fork\n");exit(1);}


		if(p[k]==0){ //codigo para o processo filho
			close(pipeline[0]);

			int j, inicioContagem, fimContagem;				//calcular o inicio e fim de porçao vector
			inicioContagem=5000*k;
			fimContagem=inicioContagem+500;

			for(j=inicioContagem;j<fimContagem;j++){
				if(vec[j].quantity>=20){
					write(pipeline[1],&vec[j],sizeof(vendas));		//envia pro tubo pelo desc escrita

				}
			}
			close(pipeline[1]);											//fecha desc. de escrita
			exit(i);


		}


	}
	//codigo para o pai filho já fez exit
				int n;
				close(pipeline[1]);
				while((n=read(pipeline[0],&venda,sizeof(vendas)))){ //ler o conteudo do pipe que o cliente enviou
					printf("pai à espera\n");
					if(n>0){
						resultadosEncontrados[ocurrencias]=n;
						ocurrencias++;
					}
					else printf("ERRO\n");
				}
				close(pipeline[0]);						//fechar o descritor de leitura enviado do cliente



	int j=0;
	printf("produtos com vendas >= 20 unidades\n");
	for(j=0;j<10;j++){
			waitpid(p[j],&status,0);
			printf("contador %d\n",ocurrencias);
			//for(i=0;i<contadorProdutos;i++)
			//printf("produto %d vendeu mais que 20\n",resultadosEncontrados[i]);

		}
 return 0;
}


