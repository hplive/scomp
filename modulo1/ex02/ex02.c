#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
/*
int main(void){
	pid_t a,b,c;
	
	a=fork();
	
	if(a>0){
		printf("I'm...%d \n",a);
		b=fork();
		if(b>0){
				printf("the...%d\n",b);
				c=fork();
				if(c>0){
					printf("father!%d\n",c);
				}
				else
					printf("I'll never join you!\n");

		}
		else
			printf("I'll never join you!\n");
	}



	return 0;
}
*/
int main(void){
	pid_t a,b,c;

	a=fork();

	printf("\n");
	if(a>0){
		printf("I'm...%d \n",a);

	}
	else{
		b=fork();
		if(b>0){
				printf("I'll never join you!\n");
				printf("the...%d\n",b);
		}
		else{
				c=fork();

				if(c>0){
					printf("I'll never join you!\n");
					printf("father!%d\n",c);
				}
				else
					printf("I'll never join you!\n");


		}

	}




	return 0;
}
