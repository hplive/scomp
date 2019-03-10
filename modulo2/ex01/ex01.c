#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main()
{
	int fd[2];
	pipe(fd);
	pid_t p, myPid, parentPid;

	p=fork();
	if(p>0){ /*codigo do pai*/
		close(fd[0]);
		myPid=getpid();
		printf("o meu pid é %d e eu sou o  pai\n",myPid);

		write(fd[1],&myPid,sizeof(int));

		close(fd[1]);
	}
	else /*codigo para o filho*/
	{
		close(fd[1]);
		read(fd[0],&parentPid,sizeof(int));
		printf("o pid do pai é %d\n",parentPid);
		close(fd[0]);
	}

 return 0;
}

