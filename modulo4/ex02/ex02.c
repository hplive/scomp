
/*
 2. Change the last program in a way that the child process reads
 from the file “Numbers.txt” by order of its process number (1 to 8).
 Use 8 semaphores to synchronize between the child processes
*/


#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>

#define N_PROCESSOS 8

int main(int argc, char *argv[]) {

	sem_t *semaforo[N_PROCESSOS];
	pid_t pid[N_PROCESSOS];
	int idp, i, status, tamanho;
	char ch;


	//*******GERAR NUMEROS NO FICHEIRO numbers.txt***************
	FILE *fileopen;							//ler ficheiro
	fileopen = fopen("numbers.txt","w");
	if( fileopen == NULL ){ 				//abrir ficheiro numbers. "a" Acrescenta a um arquivo. Operações de gravação, anexar dados no final do arquivo. O arquivo é criado se não existir.
		printf("Erro ao abrir ficheiro\n");				//mensagem caso dê NULL
	exit(1);								//terminar deviado a erro
	}
	// gerar os  numeros
	tamanho=200*N_PROCESSOS;
	for (i=0;i<tamanho;i++){
		fprintf(fileopen, "%d ", i);	//escrever no ficheiro
	}
	 fclose(fileopen);						//fechar ficheiro
	 //**********************************************************

	/* criar semáforos com valor = 0 */
	char buf[12];
	for (i = 0; i < N_PROCESSOS; i++) {
		sprintf(buf, "semaforo%d", i+1);
		if ((semaforo[i] = sem_open(buf, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
			perror("No sem_open()");
			exit(1);
		}
	}
	/* incrementa 1º semaforo */
	sem_post(semaforo[0]);

	for (idp = 0; idp < N_PROCESSOS; idp++) {

		pid[idp] = fork();

		/* verifica se foi criado */
		if (pid[idp] < 0) {
			perror("fork() error");
		}
		if (pid[idp] ==0 )
			break;
	}

	/* filho */
	if (pid[idp] == 0) {
		/* decrementa o semaforo */
		sem_wait(semaforo[idp]);
		printf("\n O Filho nº %i iniciou a leitura e escrita - pid do filho = %d\n",idp+1,getpid());
		FILE *fileOutput;
		fileOutput = fopen("output.txt","a");	//abre ou cria ou ficheiro para "appending "
		FILE *fileopen;
		fileopen=fopen("numbers.txt","r"); //abre ou cria ou ficheiro para "reading  "
		if(fileopen == NULL) {
			perror("Erro. Ficheiro de leitura nao encontrado\n");
		    exit(EXIT_FAILURE);
		}

		int limit_count=200*(1+idp), start_count=200*(idp), normal_count=start_count; //contador dos 200 numeros que cada um tem que ler
		fprintf(fileOutput,"-----------------filho %d --------------- ",idp+1);

		while (!feof (fileopen)) {
			fscanf (fileopen, "%d", &i);
			printf("%d - ",idp+1);
			if(normal_count>=start_count){
				fprintf(fileOutput,"%d ",i);
			}
			if(normal_count>=limit_count) break;
			normal_count++;
		}

		fclose(fileOutput);
		fclose(fileopen);

		/* incrementa o semaforo */
		sem_post(semaforo[idp]);

		/* incrementa o semaforo do processo seguinte */
		if (idp < N_PROCESSOS-1) {
			sem_post(semaforo[(idp + 1)]);
		}

		exit(idp + 1);
	}


	/* pai */

	/* espera por todos os filhos */
	for (idp = 0; idp < N_PROCESSOS; idp++) {
		waitpid(0, &status, 0);
		if (WIFEXITED(status)) {
			if (status != 0) {
				printf("Pai: terminou o filho %d.\n", WEXITSTATUS(status));
			}
		}
	}


	FILE *fileOutput;
	fileOutput = fopen("output.txt","r");

	while((ch=fgetc(fileOutput))!=EOF){
		printf("%c",ch);
	}
	fclose(fileOutput);

	/* fechar todos os semaforos */

	int r = sem_unlink("semaforo1");
	if (r < 0) {
		perror("semaforo1 unlink error!");
	}

	r = sem_unlink("semaforo2");
	if (r < 0) {
		perror("semaforo2 unlink error!");
	}

	r = sem_unlink("semaforo3");
	if (r < 0) {
		perror("semaforo3 unlink error!");
	}

	r = sem_unlink("semaforo4");
	if (r < 0) {
		perror("semaforo4 unlink error!");
	}
	r = sem_unlink("semaforo5");
	if (r < 0) {
		perror("semaforo5 unlink error!");
	}
	r = sem_unlink("semaforo6");
		if (r < 0) {
			perror("semaforo6 unlink error!");
	}
	r = sem_unlink("semaforo7");
		if (r < 0) {
			perror("semaforo7 unlink error!");
	}
	r = sem_unlink("semaforo8");
		if (r < 0) {
			perror("semaforo8 unlink error!");
	}

	return 0;
}


