/*
 * . Implement a program that outputs prime numbers. Your program should ask the user for an
 * unsigned integer number. The program will then create another thread that outputs all the
 * prime numbers that are less than or equal
to the number entered by the user
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int nr;

void numeros_primos( void *arg){
	int *n =(int *)arg;
	printf("numeros primos de %d ::::::::::::::::\n",*n);
	for (int i=2; i<*n; i++)
	    {
	        int prime=0;
	        for (int j=2; j*j<=i; j++)
	        {
	            if (i % j == 0)
	            {
	                prime=1;
	                break;
	            }
	        }
	        if(prime!=0) printf("%d ", i);
	    }
	printf("\n");
}

int main(){
	int s;
	pthread_t threads;

	printf("Introduza um numero :::");
	scanf("%d",&nr);
	printf("\n");

	s = pthread_create(&threads, NULL, numeros_primos, (void *) &nr);

	return 0;
}



