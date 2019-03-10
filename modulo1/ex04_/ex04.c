#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void) {
	int a=0, b, c, d;
	b = (int) fork();
	c = (int) getpid(); /* getpid(), getppid(): unistd.h*/
	d = (int) getppid();
	a = a + 5;
	printf("\na=%d, b=%d, c=%d, d=%d\n",a,b,c,d);
	return 0;
}


/*
 a) - só a variavel a terá o mesmo valor nos dois processos pois a variavel nao seofre alteração depois de ser clonada.
 	- o b no pai fica com o pid do filho e no filho o b fica zero.
 	- o c fica com o pid do filho que é igual ao b no pai
 	- o d fica com o pid do pai
b)

|a=0
|
|_____b=fork();____
|b=28421,			|b=0
|c=28420,			|c=28421,
|d=28419			|d=1332
|a=0+5=5			|a=0+5=5




 */
