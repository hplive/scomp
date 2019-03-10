#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/*
 *Write a program that simulates a betting system. Assume that the child process starts the game with a credit of 20
euros. The game has the following rules:
a. The parent process generates a random number between 1 and 5;
b. Then, it writes 1 in the pipe, notifying the child that it can make another bet, or 0, if the child’s credit
ended;
c. The child process reads the number sent by the parent process and makes a bet or terminates, accordingly.
To make a bet, the child should generate a random number between 1 and 5 and send it to the parent
process;
d. The parent process waits for the child’s bet or by its termination, accordingly. If the child’s bet is right, its
credit grows 10 euros. If the bet is wrong, its credit diminishes 5 euros;
e. The parent process sends the new credit’s value to the child process. The child process prints it.
*/

int main(){
	pid_t p;
	int fd1[2], fd2[2];
	int childMoney= 20;
	int numSorteado, flag = 0, status;
	time_t t;
	srand((unsigned)time(&t));

	if(pipe(fd1) == -1){
		perror("Pipe1 falhou.");
		return -1;
	} else if( pipe(fd2) == -1){
		perror("Pipe2 falhou.");
		return -1;
	}

	p = fork();
	if(p == -1){
		perror("Fork failed.");
		exit(-1);
	}

	if(p>0){
		close(fd1[0]);
		close(fd2[1]);
	} else {
		close(fd1[1]);
		close(fd2[0]);
	}
	do{
		if(p>0){ //Pai

		printf("Saldo: %d\n", childMoney);
		numSorteado=rand()%5;
		int flagSent = 1; //flag para indicar que pode enviar
		int numExtraido;

		write(fd1[1], &flagSent, sizeof(int));
		read(fd2[0], &numExtraido, sizeof(int));

		if(numExtraido == numSorteado){
				printf("Número sorteado %d, numero extraído: %d -> Ganhou 10€\n",numSorteado, numExtraido);
				childMoney += 10;
		}else if(numExtraido != numSorteado){
				printf("Número sorteado %d, numero extraído: %d -> Perdeu 5€\n",numSorteado, numExtraido);
				childMoney -= 5;
		}

		if(childMoney <=0){
			flag=1;
			flagSent = 0;
		}
		}else if(p == 0){
			int numEnviado;
			read(fd1[0], &numEnviado, sizeof(int));
			if(numEnviado != 0){
				int numApostado = rand()%5;
				write(fd2[1], &numApostado, sizeof(int));
			}else{
			close (fd1[1]);
			close (fd2[1]);
			exit(0);
		}
		}
	}while(flag != 1);
	printf("Ficou sem dinheiro!!!! \n");
	waitpid(p,&status,0);
	return 0;
}
