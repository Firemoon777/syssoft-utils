#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define ENG 26
char alpha[ENG] = "abcdefghijklmnopqrstuvwxyz";

pthread_t t1, t2;
sem_t sem_rev, sem_ch;

void print_alpha(void) {
	int i;
	for(i = 0; i < ENG; i++) {
		printf("%c", alpha[i]);
	}
	printf("\n");
}

void* reverse(void* d) {
	(void)d;
	int i;
	char t;
	while(sem_wait(&sem_rev) == 0) {
		for(i = 0; i < ENG / 2; i++) {
			t = alpha[i];
			alpha[i] = alpha[ENG - i - 1];
			alpha[ENG - i - 1] = t;
		}
		print_alpha();
	}
	return NULL;
}

void* chcase(void* d) {
	(void)d;
	int i;
	while(sem_wait(&sem_ch) == 0) {
		for(i = 0; i < ENG; i++) {
			alpha[i] += (alpha[i] - 'A') < ENG ? 32 : -32;
		}
		print_alpha();
	}
	return NULL;
}

int main(void) {
	if(sem_init(&sem_rev, 0, 0) == -1) {
		perror("sem_init: sem_rev");
		return -1;
	}
	if(sem_init(&sem_ch, 0, 0) == -1) {
		perror("sem_init: sem_ch");
		return -1;
	}
	pthread_create(&t1, NULL, reverse, NULL);
	pthread_create(&t2, NULL, chcase, NULL);
	while(1) {
		sem_post(&sem_rev);
		sleep(1);
		sem_post(&sem_ch);
		sleep(1);
	}
	return 0;
}
