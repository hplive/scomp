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
	int i,j, auxpid, status, posicao;
	pid_t pidProcessos[5];
	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));
	/* initialize array with random numbers (rand(): stdlib.h) */
	for (i = 0; i < ARRAY_SIZE; i++)
	{
		numbers[i] = rand () % 255;
	}

	/*criar 5 processos filhos
	 */
	for(i=0; i<5; i++){
		pidProcessos[i]=fork();
		int procurar=ARRAY_SIZE/5;
	}


return 0;
}
