#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

/*
 * Write a program that initializes an array of 2 000 random integers. Then,
 * the program should create 10 child processes that will find the first
 * occurrence of a number n and return the relative index where it was found
 * within the section of the array it computed (that is, the index returned is
 * a value in the range [0,200[). If no number is found, the child returns 255.
 * The parent should wait for the child processes to finish and output the valid
 * indexes returned.
 */
#define ARRAY_SIZE 2000

int main(void){
	int numbers[ARRAY_SIZE];
	int n,i,j,k, index,status, auxpid;
	time_t t;
	pid_t p[10];
	srand((unsigned)time(&t));
	for(i=0; i<ARRAY_SIZE;i++){
		numbers[i]=rand()%2000;
	}
	n=numbers[1000]; //fixei a procura para ser mais fácil encontrar
	printf("Procurar numero %d\n", n);
	for(i=0;i<10;i++){
		index=0;
		p[i]=fork();		//clonei processo
			if(p[i]==-1){
				perror("Fork failed."); //se valor devolvido pelo fork for -1 erro na criação do clone processo
				exit(-1);
			}else if(p[i]==0){			 //procura por parte do filho
				for(j=i*200;j<(i+1)*200;j++){
					if(numbers[j]==n){
						exit(index);
					}
					if(index==199){			/*se o ciclo terminou a sua secção deve devolver o "erro" 255*/
						exit(255);
					}
					index++;
				}
			}
	}
	for(k=0;k<10;k++){
		if(p[k]>0){ 			//inicia código do pai
			do{
				auxpid=waitpid(p[k], &status, WNOHANG); //espera pelo término do filho
				if(auxpid==-1){
					perror("Error Waitpid\n");
					exit(-1);
				}
			}while(auxpid==0);
			if(WIFEXITED(status)){ //verifica se teminou processo sem ser zero
				if(WEXITSTATUS(status)==255){
					printf("Pai: o filho %d não encontrou o  numero %d\n",auxpid, n);
				}else{
					printf("Pai: o filho %d encontrou o número %d na posição %d.\n",auxpid, n,WEXITSTATUS(status));
				}
			}
		}
	}
	return 0;
}
