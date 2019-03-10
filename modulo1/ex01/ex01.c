#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	int x = 1;
	pid_t p = fork(); /*pid_t: sys/types.h; fork(): unistd.h*/
	if (p == 0) {
		x = x+1;
		printf("1. x = %d\n",x);
	}else{
		x = x-1;
		printf("2. x = %d\n",x);
	}
	printf("3. %d; x = %d\n",p, x);
	
	return 0;

}

/*
 * a)
 * Assumindo que o filho teria o valor 1234 o exemplo de output seria
 * 1. x = 2
 * 3. 0; x = 2 - output do filho
 * 2. x = 0
 * 3. 1234; x = 0 - output do pai
 *
 * b)
 * Não é garantido que o que a linha 9 seja impressa antes da 11 pois
 * não existe nada no código que o possa impedir
 */
