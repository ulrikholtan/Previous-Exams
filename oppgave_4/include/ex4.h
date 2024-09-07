#ifndef EX4_H
#define EX4_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#include "sha1.h"

#define BUFFER_SIZE 4096
#define NUM_THREADS 2
#define BYTE_RANGE 256

struct thread_data {
	pthread_mutex_t mutex;
	sem_t sem_full, sem_empty;
	unsigned char buffer[BUFFER_SIZE];
	int bytes_in_buffer;
	int count[BYTE_RANGE];
	FILE* file_pointer;
	int threads_completed;
};

void* thread_A(void* arg);
void* thread_B(void* arg);
int main(int argc, char* argv[]);

#endif /* TEX4_H */

