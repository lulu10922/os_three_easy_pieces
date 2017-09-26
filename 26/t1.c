#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "mythreads.h"

static int counter = 0;
void *mythread(void *arg){
	int i;
	for(i = 0;i < 1e7;i++)
		counter++;
	return NULL;
}

int main(int argc, char *argv[]){
	pthread_t p1, p2;
	printf("main: begin\n");
	Pthread_create(&p1, NULL, mythread, "A");
	Pthread_create(&p2, NULL, mythread, "B"); 
	
	Pthread_join(p1, NULL);
	Pthread_join(p2, NULL); 
	printf("counter = %d\n", counter);
	printf("main: end\n");
	return 0;
}