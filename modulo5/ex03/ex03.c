#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


#define SIZEARRAY 1000
#define NPOSITIONS 200
#define NTHREAD 5

typedef struct {
	int *array;
	int position;
    int number;
} valores;

void* thread_func(void *arg) {

	valores *v = (valores*)arg;

	int i;

	for(i=(v->position*NPOSITIONS);i<((v->position+1)*NPOSITIONS);i++) {

		if(v->array[i] == v->number) {
			int *valor=(int*)malloc(sizeof(int));
			printf("Encontrei %d na posicao %d do vetor \n",v->number,i+1);
			*(valor)=v->position+1;
			pthread_exit((void*)valor);

		}
	}
	pthread_exit((void*)NULL);
}

int main() {


	int vec[SIZEARRAY], i, num;
	pthread_t threads[NTHREAD];
	void *test;

	valores args[5];

	// coloca valores no array
	srand(time(NULL));
    for(i=0;i<SIZEARRAY;i++){
        vec[i] = rand()%SIZEARRAY + 1;
    }

	printf("Defina o numero a procurar: ");
	scanf("%d", &num);


	for(i=0;i<NTHREAD;i++){
		args[i].array=vec;
		args[i].position=i;
		args[i].array=vec;
        pthread_create(&threads[i],NULL,thread_func,(void*)&args[i]);
    }

	for(i=0; i<5; i++) {
		pthread_join(threads[i], NULL);

	if(test != NULL) {
		int tmp=*((int*)test);
		printf("Thread que encontrou o numero %d foi a thread: %d \n",args[i].number,tmp);
    }

	}

	free(test);
	return EXIT_SUCCESS;
}
