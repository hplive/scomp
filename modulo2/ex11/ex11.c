/*
 * 11. Write a program that creates 5 child processes. Connect all 6 processes with a “ring”
 * topology through pipes: the parent process will be connected to child 1, child 1 is connected
 * to child 2, …, and child 5 is connected to the parent process. The goal is to find the greatest
 *  random number generated by all processes:
 *  a. Each process generates a random number between 1 and 500. Then, it prints it along with its PID;
	b. Then, the parent process sends its generated number to child 1;
	c. Child 1 compares the parent’s number with its own random number and sends the greater of the two to
		child 2;
	d. All other processes follow the same behavior, until child 5 sends the greater number to the parent process;
	e. The parent process prints the greatest random number.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int randomNumber();
int main()
{
	int fd[6][2];
	int biggestNr=0;
	int numberRadomized=randomNumber(1);
	pid_t p[5];
	int numero=getpid();

	printf("Pid: %d | Number Generated :%d | PAI\n",getpid(), numberRadomized);
	biggestNr=numberRadomized;

	pipe(fd[0]);
	write(fd[0][1], &biggestNr, sizeof(int));
	close(fd[0][1]);								//fechar descritor escrita do primeiro pipe

	int i;
	for(i=0;i<5;i++){
		p[i] = fork ();
		if(i==4) pipe(fd[4]);
		if(p[i]==0){
			int number_random=randomNumber(i);
			//printf("Pid: %d | Number Generated :%d \n",getpid(), number_random);

			read(fd[i][0], &biggestNr,sizeof(int));
			close(fd[i][0]);
			if(biggestNr>=number_random){
				printf("pid: %d |Biggest number is the last one -- %d\n", getpid(),biggestNr);
			}else{
				printf("pid %d | Number generated in this pid is bigger -- %d\n",getpid(), number_random);
				biggestNr=number_random;
			}
			//sleep(1);
			if(i<4){
				if(i<3)pipe(fd[i+1]);
				write(fd[i+1][1],&biggestNr, sizeof(int));						//envia o maior mnumero
				close(fd[i+1][1]);
			}else{
				write(fd[4][1],&biggestNr, sizeof(int));						//envia o maior mnumero
			}

			exit(i);

		}

	}

	sleep(2);
	//last call :)
	close(fd[4][1]);												//fechar escrita
	read(fd[4][0], &biggestNr , sizeof(int));
	close(fd[4][0]);												//fechar leitura

	printf("PID %d| The BIGGEST RANDOM NUMBER is %d\n",getpid(),biggestNr);




	return 0;

}

int randomNumber(int i){
	//srand ( time(NULL) );
	//return(rand ());
	return i+1;
}
