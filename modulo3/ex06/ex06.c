/*. Using the program written for question 5, determine which is the number of processes which gives the best
performance in your systems. Discuss the results taking into account the architecture of your computer.
*/

#include <sys/mman.h>
#include <sys/stat.h> /* Para constantes de “modo” */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define operations 10000000


typedef struct{
	int num;
	int flag;
}areaStore;


int main() {

    printf("#####################################\n waiting for five :( \n##########################################\n");




    return 0;
}


