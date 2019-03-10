#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

/*
 * Given the code below, write the code necessary to create a child process to
 * find how many times the number n is found in half of the elements
 * of the numbers array. While the child is processing, the parent process should
 * search the other half.
 * After both finish processing their half, the parent computes and presents how
 * many times n was found in the entire array (assume that no number is
 * repeated more than 255 times).
 */
#define ARRAY_SIZE 1000

int main(void){
	int numbers[ARRAY_SIZE];
	int n,i,status, countfilho=0, countpai=0, countgeral=0, auxpid;
	time_t t;
	pid_t p;
	srand((unsigned)time(&t));
	for(i=0; i<ARRAY_SIZE;i++){
		numbers[i]=rand()%10000;
	}
	n=numbers[5]; //fixei a procura para ser mais fácil encontrar
	p=fork(); 			//clonei processo
	if(p==-1){
		perror("Fork failed."); //se valor devolvido pelo fork for -1 erro na criação do clone processo
		exit(-1);
	}else if(p==0){			 //procura por parte do filho
		for(i=0;i<ARRAY_SIZE/2;i++){
			if(numbers[i]==n){
				countfilho++;
			}
		}
		exit(countfilho); //termina o processo com o contador
	}else if(p>0){ 			//inicia código do pai
		do{
			auxpid=waitpid(p, &status, WNOHANG); //espera pelo término do filho
			if(auxpid==-1){
				perror("Error Waitpid\n");
				exit(-1);
			}
		}while(auxpid==0);

		for(i=ARRAY_SIZE/2; i<ARRAY_SIZE; i++){ //procura o numero na segunda metade do array
			if(n==numbers[i]){
				countpai++;
			}
		}
		if(WIFEXITED(status)){ //verifica se teminou processo sem ser zero
			printf("Pai: o filho %d encontrou o  numero %d, %d vezes \n",auxpid, n,WEXITSTATUS(status));
			printf("Pai: encontrei o  numero %d, %d vezes \n", n,countpai);
			countgeral=countpai+WEXITSTATUS(status);
			printf("O numero %d, foi encontrado %d vezes", n, countgeral);
		}
	}
	return 0;
}
