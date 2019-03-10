#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

/*
 * 6. Given the following code.
 */

int main(void){
	int i;
	//int status;

	for(i=0;i<4;i++){
		if (fork() == 0){
			sleep(1); /*sleep(): unistd.h*/
		}
	}
	printf("This is the end.\n");
	return 0;
}

/*
 * a) How many processes will be created by this code? Justify by drawing a process tree that describes the processes created.
 * Serão criados 16 processos
 *
 *      |
 *	i=0 |_______________________________________________________
 *	    |      		 	 										|
 *	i=1 |___________________________						    |___________________________
 *	    |   	      		  		|    						|							|
 *	i=2 |___________   	 			|_______________			|_______________			|_______________
 *	    |     		|       	  	|	 	 	 	|    	    |				|           |				|
 *	    |     	 	|      			|	   			|			|				|			|				|
 *  i=3 |__    	 	|__    			|___   			|__			|___			|___ 	    |___	   	    |__
 *      |  |	    |  |        	|	|   	    |  |	    |	| 	    	|	|   	|	|	    	|	|
 *	    |  |   		|  |    		|	|  			|  |		|	|			|	|		|	|			|	|
*/
