/*
 * Given the code below, write the code necessary to create a child process to
 * find how many times the number n is found in half of the elements of the
 * numbers array. While the child is processing, the parent process should
 * search the other half. After both finish processing their half, the parent
 * computes and presents how many times n was found in the entire array
 * (assume that no number is repeated more than 255 times).
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#define ARRAY_SIZE 1000
int main ()
{
int numbers[ARRAY_SIZE]; 	/* array to lookup */
int n; 						/* the number to find */
time_t t; 					/* needed to init. the random number generator (RNG)*/
int i, count, auxpid, status;
pid_t pid;

/* intializes RNG (srand():stdlib.h; time(): time.h) */
srand ((unsigned) time (&t));
/* initialize array with random numbers (rand(): stdlib.h) */
for (i = 0; i < ARRAY_SIZE; i++)
{
	numbers[i] = rand () % 1000;
	/* initialize n */
	n = rand () % 1000;
}

int metade=ARRAY_SIZE/2;		//metade do array para
n=numbers[metade];				//alterei o numero n porque senao dificilmente encontra algum
pid=fork();						//criar novo processo
if(pid==0){						//codigo para o filho pesquisar a segunda metade do array
	for(i=metade;i<ARRAY_SIZE;i++){	//ciclo para pesquisar a segunda metade do array
		if(numbers[i]==n)
			count++;			//contador, ate aqui o contador está a zero
	}
	exit(count);				//retorno do contador do filho
}
if(pid>0)						//codigo para o pai
{
	do{							//esperar que o filho termine
		auxpid=waitpid(pid,&status, WNOHANG);
		if (auxpid==-1)
		{
			perror("Erro em waitpid");
		exit(-1);
		}
		}while(auxpid==0);

		//pesquisa da primeira metade do pai
		for(i=0;i<metade;i++){
			if(numbers[i]==n)
				count++;
		}
		if(WIFEXITED(status))
		printf("Pai: o filho %d encontrou o  numero %d, %d vezes \n",auxpid, n,WEXITSTATUS(status));
		printf("Eu sou o pai  e encontrei o  numero %d, %d vezes \n", n,count);
		count=count+WEXITSTATUS(status);
		printf("O numero %d, foi encontrado %d vezes", n, count);

}

return 0;
}
