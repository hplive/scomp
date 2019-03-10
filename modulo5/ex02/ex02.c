/*
Consider an array, in which each array element is a data structure. The fields of
such data structure are: number,name and address. The array has a capacity for 5
elements. Implement a function to receive as parameter one
instance of such data structure. The function must print the content of the
received data structure parameter.
Assumptions:
 Create 5 threads;
 Each thread must start in the created function;
 Each thread will only deal with an element of the array.

*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>
#include <inttypes.h>

typedef struct{
	int number;
	char name[50];
	char address[100];
} data_type;


void * escreve(void *arg)
{
	data_type *var = (data_type *) arg;
	printf("%s | %s | %d \n", var->address,var->name,var->number);
	pthread_exit(NULL);
}
int main()
{
	data_type array[5]; //array pedido no enunciado
	//inicialização do array
	sprintf(array[0].address,"Rua um"); sprintf(array[0].name,"ana");array[0].number=5;
	sprintf(array[1].address,"Rua dois"); sprintf(array[1].name,"bruno");array[1].number=4;
	sprintf(array[2].address,"Rua tres"); sprintf(array[2].name,"carlos");array[2].number=3;
	sprintf(array[3].address,"Rua quatro"); sprintf(array[3].name,"daniel");array[3].number=2;
	sprintf(array[4].address,"Rua cinco"); sprintf(array[4].name,"eduarda");array[4].number=1;



	pthread_t threads[5];
	int s, i;
	for (i = 0; i < 5; i++)
	{
		printf("A criar o thread n. %d\n", i);
		s = pthread_create(&threads[i], NULL, escreve, (void *) &array[i]);
		if (s!=0) {
		perror("Erro ao criar o thread");
		exit(-1);
	}
}
pthread_exit(NULL);
}
