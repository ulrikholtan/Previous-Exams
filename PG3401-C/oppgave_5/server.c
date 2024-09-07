#include "server.h"

/* Hovedfunksjonen for oppsett av serveren */
int main(int argc, char *argv[]) {
	int iPorts[2];
	pthread_t threads[2];
	int sockFd[2];

	/* Legg inn sjekk for parametere */
	if (argc < 4) {
		fprintf(stderr, "Usage: %s -listen <port>\n", argv[0]);
		return 1;
	}

	iPorts[0] = atoi(argv[2]);
	iPorts[1] = atoi(argv[3]);

	printf("Listening on port: %d\n", iPorts[0]);
	printf("Listening on port: %d\n", iPorts[1]);

	/* Lag to tråder for å håndtere tilkoblingen for hver port */
	for (int i = 0; i < 2; ++i) {
		struct sockaddr_in saAddr = {0};
		int addrLen;

		/* Opprett sockets */
		sockFd[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (sockFd[i] < 0) {
			fprintf(stderr, "Socket creation failed with %d\n", errno);
			return 1;
		}

		/* Adresse struktur */
		saAddr.sin_family = AF_INET;
		saAddr.sin_port = htons(iPorts[i]);
		saAddr.sin_addr.s_addr = htonl(0x7F000001);

		/* BIND socket */
		if (bind(sockFd[i], (struct sockaddr *)&saAddr, sizeof(saAddr)) < 0) {
			printf("Bind failed with %i\n", errno);
			close(sockFd[i]);
			return 1;
		}

		/* Lytt etter tilkobling */
		listen(sockFd[i], 2);

		/* Godta tilkobling */
		addrLen = sizeof(saAddr);
		int *sockNewFd = malloc(sizeof(int));
		*sockNewFd = accept(sockFd[i], (struct sockaddr *)&saAddr, (socklen_t *)&addrLen);
		if (*sockNewFd < 0) {
			printf("Accept failed with %i\n", errno);
			close(sockFd[i]);
			free(sockNewFd);
			return 1;
		}

		/* Opprett tråd for å håndtere tilkobling */
		if (pthread_create(&threads[i], NULL, handleConnection, (void *)sockNewFd)) {
			fprintf(stderr, "error creating thread\n");
			close(*sockNewFd);
			free(sockNewFd);
			return 0;
		}
	}

	/* Vent på at de skal bli ferdig*/
	for (int i = 0; i < 2; ++i) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}

/* Håndtering av innkommende connections */
void *handleConnection(void *sockFdPtr) {
	int sockNewFd = *(int *)sockFdPtr;
	TASK5_CONNECT_USR recvMsg;
	int bytesRead;

	/* Les connection melding først */
	if (read(sockNewFd, &recvMsg, sizeof(recvMsg)) != sizeof(recvMsg)) {
		fprintf(stderr, "failed to recieve full connect message\n");
		close(sockNewFd);
		return NULL;
	}

	printf("Received connection from IP: %d.%d.%d.%d, Phone: %d\n", 
		(recvMsg.iIpAddress >> 24) & 0xFF, 
		(recvMsg.iIpAddress >> 16) & 0xFF, 
		(recvMsg.iIpAddress >> 8) & 0xFF, 
		recvMsg.iIpAddress & 0xFF, 
		recvMsg.iPhoneNumber);

	/* Ta imot header for meldingen og les meldingen */
	while (1) {
		TASK5_SENDMESSAGE header;
		bytesRead = read(sockNewFd, &header, sizeof(int) * 2); /* Les kun iMagicNumber iMessageSize */

		if (bytesRead < (int)(sizeof(int) * 2)) {
			if (bytesRead == 0) {
				printf("Client %d closed connection\n", recvMsg.iPhoneNumber);
				break;
			}
			fprintf(stderr, "incomplete header recieved\n");
			close(sockNewFd);
			break;
		}

		if (ntohl(header.iMagicNumber) != MAGIC_NUMBER) {
			fprintf(stderr, "Magic number does not match.\n");
			break;
		}

		int messageSize = (ntohl(header.iMessageSize) - sizeof(int) * 2);
		if (messageSize <= 0 || messageSize > MAX_MESSAGE_SIZE) {
			fprintf(stderr, "Invalid message size: %d\n", messageSize);
			break;
		}

		char *messageBuffer = malloc(messageSize + 1); /* alloker plass til meldingen */
		if(!messageBuffer) {
			perror("Failet å allokere minne for melding");
			break;
		}

		bytesRead = read(sockNewFd, messageBuffer, messageSize);
		if (bytesRead < messageSize) {
			fprintf(stderr, "Inclompete message recieved. Expected %d bytes, got %d bytes\n", messageSize, bytesRead);
			free(messageBuffer);
			break;
		}
		messageBuffer[messageSize] = '\0'; /* Legg på nul terminering */

		printf("Number %d: %s\n", recvMsg.iPhoneNumber, messageBuffer);

		free(messageBuffer);
	}

	/* Frigjør minne for sockNewFd */
	free(sockFdPtr);
	close(sockNewFd);
	return NULL;
}
