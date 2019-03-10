#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
 int i;
 for (i=1; i<4; i++) fork();
 execlp("echo","echo", "SCOMP", NULL);
 return 0;
}

/*******************************

 a)
 |
 |i=1;fork();
 |__________________________________________
 |											|
 |											|
 |i=2 fork()_______							|i=2 fork()__________
 |					|						|					|
 |					|						|					|
 |					|						|					|
 |



 */
