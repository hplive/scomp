#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>




int main(int argc, char** argv)
{
	int i, n, fact = 1;
	n= atoi(argv[1]);
	for (i = 1; i <= n; i++)
		    fact = fact * i;
		printf("%d\n",fact);
	return 0;
}
