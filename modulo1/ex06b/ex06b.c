#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

/*
 * 6. Given the following code.
 */

int main(void){
	int i, f=0;
	pid_t p;
	for(i=0;i<4;i++){
		p=fork();
		f++;
		if(p==0){ //é filho
			sleep(1); /*sleep(): unistd.h*/
			printf("Filho nº %d\n", f);
			exit(1);
		}
	}
	printf("This is the end.\n");
	return 0;
}

/*
 * b) What change, if any, would you do to this code so that exactly 4 child processes are created?
 * f++;
 * 		if(p==0){ //é filho
 * 				sleep(1);
 * 				printf("Filho nº %d\n", f);
 * 				exit(1);
*/

