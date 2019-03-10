
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "time.h"

int generateNumber(int n){
	int number = 0;
	number = (rand() % n); //gera numeros de 0 até r
	return number;
}
