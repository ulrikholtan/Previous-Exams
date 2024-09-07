/* 
Vanligvis har jeg brukt Hungarian Notation i oppgaver, som også er nevnt i forelesning.
Ettersom kildefilen benyttet seg av "snake_case", 
har jeg valgt å fortsette med dette og ikke refactore koden.
*/

#include "include/ex4.h"

/* Hovedfunksjon, setter opp tråder og synkroniserer */
int main(int iArgc, char* szCargv[]) {
	/* Sjekk om programmet blir kjørt korrekt, med en fil */	
	if (iArgc < 2) {
		printf("Syntax error. Run the program like this: ./ex4 <yourfile.txt>\n");
		exit(1);
	} 

	pthread_t threadA, threadB;
	struct thread_data data;
	/* Åpne filen */
	data.file_pointer = fopen(szCargv[1], "rb");
	if (!data.file_pointer) {
		perror("Could not open read file");
		exit(1);
	}

	/* Initialisering av mutex og semaforer */
	if (pthread_mutex_init(&data.mutex, NULL) != 0) {
		perror("Initialization av mutex failed");
		fclose(data.file_pointer);
		exit(1);
	}

	if (sem_init(&data.sem_full, 0, 0) != 0) {
		perror("Initialization of sem_full failed");
		fclose(data.file_pointer);
		pthread_mutex_destroy(&data.mutex);
		exit(1);
	}

	/* Initialiser bufferet til tomt */
	if (sem_init(&data.sem_empty, 0, 1) != 0) {
		perror("Initialization of sem_empty failed");
		fclose(data.file_pointer);
		pthread_mutex_destroy(&data.mutex);
		sem_destroy(&data.sem_full);
		exit(1);
	}

	data.bytes_in_buffer = 0;
	/* Sett fullførte tråder til 0 */
	data.threads_completed = 0;

	/* Opprett tråder */
   if (pthread_create(&threadA, NULL, thread_A, (void*)&data) != 0) {
      perror("Could not create thread A");
		fclose(data.file_pointer);
		pthread_mutex_destroy(&data.mutex);
		sem_destroy(&data.sem_full);
		sem_destroy(&data.sem_empty);
      exit(1);
   }

   if (pthread_create(&threadB, NULL, thread_B, (void*)&data) != 0) {
      perror("Could not create thread B");
		pthread_cancel(threadA);
		pthread_join(threadA, NULL);
		fclose(data.file_pointer);
		pthread_mutex_destroy(&data.mutex);
		sem_destroy(&data.sem_full);
		sem_destroy(&data.sem_empty);
      exit(1);
   }

	/* Vent på at trådene fullfører */
	pthread_join(threadA, NULL);
	pthread_join(threadB, NULL);

	/* Sjekker om begge trådene er ferdige */
	pthread_mutex_lock(&data.mutex);
	printf("Threads completed: %d\n", data.threads_completed);
	if (data.threads_completed == 2) {
		printf("All threads finished.\n");
	}
	pthread_mutex_unlock(&data.mutex);

	/* Opprydding og steng ned */
	pthread_mutex_destroy(&data.mutex);
	sem_destroy(&data.sem_full);
	sem_destroy(&data.sem_empty);
	fclose(data.file_pointer); /*  Lukk filen */

   return 0;
}


/* Tråd A, som leser fra en gitt fil og laster inn i buffer */
void* thread_A(void* arg) {
	struct thread_data* p_data = (struct thread_data*)arg;

	while (1) {
		sem_wait(&p_data->sem_empty); /* Vent på at bufferet ikke er fullt */
		pthread_mutex_lock(&p_data->mutex); /* Lås mutex for å oppdatere bufferet */

		/* Nå som mutex er låst, kan vi lese data inn i bufferet */
		int read_bytes = fread(p_data->buffer, 1, BUFFER_SIZE, p_data->file_pointer);

		p_data->bytes_in_buffer = read_bytes;
		
		pthread_mutex_unlock(&p_data->mutex);
		sem_post(&p_data->sem_full); /* Si ifra at buffer er fullt */
	
		if (read_bytes < BUFFER_SIZE) { /* Hvis det er EOF */
			/* Si ifra at thread har kjørt ferdig */
			pthread_mutex_lock(&p_data->mutex);
			p_data->threads_completed++;
			pthread_mutex_unlock(&p_data->mutex);
			sem_post(&p_data->sem_full);
			break;
		}
	}
	pthread_exit(NULL);
}

/* Tråd B, som behandler data fra buffer */
void* thread_B(void* arg) {
	struct thread_data* p_data = (struct thread_data*)arg;
	SHA1_CTX ctx;
	unsigned char digest[20]; /* Lager et array for å holde på resultatet fra SHA1*/

	SHA1Init(&ctx); /* Her initialiserer jeg SHA-1 */

   while (1) {
		sem_wait(&p_data->sem_full); /* Vent på at bufferet er fullt */
      pthread_mutex_lock(&p_data->mutex);

		if (p_data->bytes_in_buffer > 0) {
			SHA1Update(&ctx, p_data->buffer, p_data->bytes_in_buffer); /* Oppdater SHA1 med buffer */
			p_data->bytes_in_buffer = 0;
		}

      pthread_mutex_unlock(&p_data->mutex);
      sem_post(&p_data->sem_empty); /* Si ifra at buffer er tomt */

		/* Sjekk om tråd A er ferdig og break hvis det ikke kommer mer data */
		if (p_data->threads_completed && p_data->bytes_in_buffer == 0) {
			pthread_mutex_lock(&p_data->mutex);
			p_data->threads_completed++;
			pthread_mutex_unlock(&p_data->mutex);
			break;
		}
   }

	SHA1Final(digest, &ctx); /* Avslutt SHA1 regningen */

	/* Print HASH i hex format */
	printf("SHA-1 Hash for text fil:\n");
	for (int i = 0; i < 20; i++) {
		printf("%02x", digest[i]);
	}
	printf("\n");   

	pthread_exit(NULL);
}

