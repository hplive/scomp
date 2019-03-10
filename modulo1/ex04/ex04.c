#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void){
	int a=0,b,c,d;
	b = (int) fork();
	c = (int) getpid();
	d = (int) getppid();
	a = a + 5;
	printf("\na=%d, b=%d, c=%d, d=%d\n", a,b,c,d);
	return 0;
}

/*
 * a)
 * Apenas a variável a fica com o mesmo valor, pois todas as outras são influenciadas
 * pelos valor após o fork, cópia ser efetuada.
 * A variável b no pai fica com o pid do filho e no filho fica a 0
 * A variável c no pai fica com o seu pid e no filho igual ao b do pai
 * A variável d no pai fica com o pid do seu pai e no filho igual ao c do pai
 *
 * b)
 * |a=0
 * |
 * |_____b=fork();______
 * |b=4808				|b=0
 * |c=4797				|c=4808
 * |d=4786				|d=4797
 * |a=a+5=5				|a=a+5=5
 *
 */
