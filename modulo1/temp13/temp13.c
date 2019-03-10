#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
int i;

for(i=0;i<10;i++)
	execlp("echo","echo", "SCOMP", NULL);

printf("a)Only print SCOMP once.\n");
printf("b) porque o exec executa um programa que neste caso é um echo e deixa de conhecer o código nativo apartir desse momento em que é invocada o codigo exec");

return 0;
}
