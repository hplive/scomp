#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


int spawn_childs(int n);

int main (void)
{
	int value,n=3;
	value=spawn_childs(n);
	printf("valor %d\n",value*2);
	return 0;
}

int spawn_childs(int n){
	int i;
	pid_t p;
	for(i=1;i<=n;i++){
		p=fork();
		if(p==0){
			break;
		}
		if(p>0)wait(p);

	}
	if(p>0) return 0;
	return i;

}
