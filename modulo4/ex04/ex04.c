
/*3.
 * Implement a program that should behave as follows:
• Guarantee mutual exclusion to the shared memory;
• Add, to the shared memory, a text line with the following information: "I’m the Father - with PID X", where X
should be replaced by the PID of the process;
• Wait N seconds (from 1 to 5 seconds);
Test the solution by executing several concurrent instances of your code.
Note: Consider that you have a shared memory area with 50 strings, each string having 80 characters.
*/
/*
 * 4. Change the last question by adding the following features:
- Add an option to “remove” (free) one string from the shared memory;
- Your program should wait at most 12 seconds to access the shared memory, warning the user if that is not possible.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "time.h"

#define BUF 80
#define NPROCESSOS 5

int main(int argc, char *argv[]) {

	sem_t *semaforo;
	struct timespec ts;
	pid_t pid[NPROCESSOS];
	int nlinhas, linha, i, idp, j, r, file_status = 1;

	FILE *file;
	char *filename = "strings.txt";

	char *linhas[50]; /* array para guardar as linhas */
	char newline[BUF];

	size_t tamanho = 0;

	/* alocar espaço para as linhas que vão ser lidas */
	for (i = 0; i < 50; i++) {
		linhas[i] = malloc(255 * sizeof *linhas[i]);
	}

	/* Definir temporizador 12 seconds */
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
	}
	ts.tv_sec += 12;

	/* criar o semáforo = 1 */
	semaforo = sem_open("semaforo", O_CREAT | O_EXCL, 0644, 1);
	if (semaforo < 0) {
		perror("sem_open() error");
		exit(-1);
	}

	for (idp = 0; idp < NPROCESSOS; idp++) {

		pid[idp] = fork();

		/* verifica se foi criado */
		if (pid[idp] < 0) {
			perror("fork() error");
		}

		/* processo filho */
		if (pid[idp] == 0) {

			nlinhas = 0;

			r = sem_timedwait(semaforo, &ts); /* decrementa o semaforo, não sendo possivel ao fim de 12s devolve erro	*/

			if (r == -1) {
				perror("12 seconds timeout\n");
				exit(-1);
			}

			printf("Vou ler do ficheiro.\n");

			/* abrir o ficheiro */
			file = fopen(filename, "a+" );
			if (file == NULL) {
				perror("Error accessing file.\n");
				file_status = 0;
				exit(-1);
			}

			/* contar linhas */
			if (file_status) {
				while (getline(&linhas[j], &tamanho, file) != -1) {
					nlinhas++;
					j++;
				}
			}

			fclose(file);

			printf("Número de linhas lidas: %d \n", nlinhas);

			/* abrir o ficheiro*/

			file = fopen(filename, "w");
			if (file == NULL) {
				printf("Error opening file!\n");
				exit(1);
			}

			/*Quando for superior a 50 podemos substituir a linha pretendida */
			if (nlinhas < 50) {
				for (j = 0; j < nlinhas; j++) {
					fprintf(file, "%s", linhas[j]);
				}
				fprintf(file, "I’m the Father - with PID %d .\n", getppid());
			} else {

				do {
					printf("Que linha deseja substituir: ");
					scanf("%d", &linha);
				} while (linha < 0 || linha > 50);

				sprintf(newline, "I’m the Father - with PID %d .\n",getppid());

				strcpy(linhas[linha], newline);

				for (j = 0; j < nlinhas; j++) {
					fprintf(file, "%s", linhas[j]);
				}

			}

			sleep(2); /* espera 2 segundos */

			fclose(file);

			printf("Processo %d, terminou.\n", idp);

			exit(i);
		}
	}

	/* processo pai */

	/* espera que os filhos terminem */
	for (i = 0; i < NPROCESSOS; i++) {
		wait(NULL);
		sem_post(semaforo); /* incrementa o semaforo */
	}

	r = sem_unlink("semaforo");
	if (r < 0) {
		perror("sem_unlink error");
	}

	return 0;
}
