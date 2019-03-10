#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

void *thread_func(void *arg){
	printf("Sou a thread %u e estou a executar\n", (int) pthread_self());
	pthread_exit((void*)NULL);
}


int main(){
	pid_t pid = fork();

	if(pid == 0){
		fork();
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, thread_func, NULL);
		pthread_join(thread_id, NULL);
		printf("Todas as threads terminaram\n");
	}
	fork();
}

/*
 * a) foram criados dois processos
 * b) foram criadas duas threads
 *
 */
