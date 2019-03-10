#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void){


	fork();fork();fork();

	printf("\nScomp! %d\n",getpid());
	return 0;
}


/*a) gera 8 processos
 * duplica os processos anterior já criados.
 *
 b)


                   			|
				        	|(fork)
							|____________
							|		    |
							|pid>0		|pid=0
							|		    |
			              	|           |
							|		    |
		     	 ___________|(fork)		|____________
			   |pid=0	    |pid>0		|pid>0       |pid=0
			   |	        |		    |            |
			   |            |			|            |
			   |	        |			|            |
		_______|       _____|(fork)		|______	     |_____	_
       |pid=0  |pid>0 |pd=0 | pid>0     |pid>0| pid=0|pid>0 |pid=0
       |       |      |     |           |     |      |      |
	 scomp	 scomp   scomp  scomp     scomp scmop  scomp  scomp

c) scomp imprime 8 vezes

 */
