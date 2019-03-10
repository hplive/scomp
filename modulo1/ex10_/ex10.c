/*
 * . Write a program that initializes an array of 2 000 random integers. Then,
 * the program should create 10 child processes that will find the first
 * occurrence of a number n and return the relative index where it was found
 * within the section of the array it computed (that is, the index returned is
 * a value in the range [0,200[). If no number is found, the child returns 255.
 * The parent should wait for the child processes to finish and output the valid
 * indexes returned.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#define ARRAY_SIZE 2000
int main ()
{
int numbers[ARRAY_SIZE]; 	/* array to lookup */
int n; 						/* the number to find */
time_t t; 					/* needed to init. the random number generator (RNG)*/
int i,j, auxpid, status, posicao;
pid_t pidProcessos[10];

/* intializes RNG (srand():stdlib.h; time(): time.h) */
srand ((unsigned) time (&t));
/* initialize array with random numbers (rand(): stdlib.h) */
for (i = 0; i < ARRAY_SIZE; i++)
{
	numbers[i] = rand () % 254;
	/* initialize n */
	n = rand () % 254;
}

int metade=ARRAY_SIZE/2;		/*metade do array para*/
n=numbers[metade];				/*alterei o numero n porque senao dificilmente encontra algum*/
int limite=200;
printf("NUMERO A PROCURAR %d*************\n",n);

for(i=0;i<10;i++){
	limite=200+200*i;

	pidProcessos[i]=fork();     		/*criar novo processo*/
	posicao=0;					/*variavel para devolver a posicao da secção do array como se fosse um array apenas*/
	printf("********************************\n");
	if(pidProcessos[i]==0){				/*codigo para o filho pesquisar na sua secção do array*/
		for(j=limite-200;j<limite;j++){	/*ciclo para pesquisar na sua secção [x,y[ de 200 posições*/
			if(numbers[j]==n){			/*se encontrar a primiera ocurrencia devolve a posição*/
				if(i!=0) printf("NA SECÇÃO [%d,%d] TEM PELO MENOS UMA OCURRENCIA\n",200*i,i*200-1+200);
				else printf("NA SECÇÃO [%d,%d] TEM PELO MENOS UMA OCURRENCIA\n",0,199);
				exit(posicao);
			}
			if(posicao==199){			/*se o ciclo terminou a sua secção deve devolver o "erro" 255*/
				printf("NA SECÇÃO [%d,%d] NAO TEM NADA\n",j-199,j);
				exit(255);
			}
			posicao++;
		}

	}
	/*codigo para o processo pai*/
	if(pidProcessos[i]>0){
		/*esperar que o filho i termine*/
		do{
				auxpid=waitpid(pidProcessos[i],&status, WNOHANG);
				if (auxpid==-1){
					perror("Erro em waitpid");
					exit(-1);
				}
			}while(auxpid==0);

		if(WIFEXITED(status)){
			int pos=WEXITSTATUS(status);

			if(pos!=255){
				pos=WEXITSTATUS(status)+200*i;
				printf("Processo filho nr %d encontrou na posição %d do array\n",i,pos);
			}
			else{
				printf("O processo filho nr %d não encontrou o numero procurado\n",i);
			}

		}



	}




}





return 0;
}
