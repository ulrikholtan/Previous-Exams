#include "client.h"

int main(int argc, char *argv[]) {
	struct sockaddr_in saAddr = {0}; // Connect
	int sockFd = -1;
	int iPort = -1;
	char *serverIp = NULL;
	char szInput[4096]; /* Buffer for å holde input */

	TASK5_CONNECT_USR connectMsg; /* Strukt for tilkoblings melding */
	TASK5_SENDMESSAGE sendMessage; /* Strukt for å sende melding */

	if (argc < 7) {
		fprintf(stderr, "Syntax: %s -server <IP> -port <port> -telephone <ID>\n", argv[0]);
		return 1;
	}

	/* Legg inn sjekk for parametere */
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-server") == 0) {
			serverIp = argv[++i]; /* Sett -server ip til argumentet */
		} else if (strcmp(argv[i], "-port") == 0) {
			iPort = atoi(argv[++i]); /* Sett -port argumentet til porten */
		} else if (strcmp(argv[i], "-telephone") == 0) {
			connectMsg.iPhoneNumber = atoi(argv[++i]);
        }
	}

	/* Sjekk om IP adresse og port er gitt */
	if (serverIp == NULL || iPort == -1) {
		fprintf(stderr, "Missing -server or -port argument\n");
		return 1;
	}

	/* Lag socket */
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFd < 0) {
		printf("socket failed with %i\n", errno);
		return 1;
	}
	
	/* Sett opp adresse som struct */
	saAddr.sin_family = AF_INET;
	saAddr.sin_port = htons(iPort);
	if (inet_pton(AF_INET, serverIp, &saAddr.sin_addr) <= 0) {
		printf("Invalid address / Address not supported \n");
		close(sockFd);
		return 1;
	}

	/* Koble til server */
	if (connect(sockFd, (struct sockaddr *)&saAddr, sizeof(saAddr)) < 0) {
		printf("connect failed with %i\n", errno);
		close(sockFd);
		return 1;
	}

	/* Lag meldingen */
	connectMsg.iMagicNumber = 0x4A454449;
	connectMsg.iMessageSize = sizeof(TASK5_CONNECT_USR);
	connectMsg.iIpAddress = ntohl(saAddr.sin_addr.s_addr);

	/* Send struct som melding */
	if (send(sockFd, &connectMsg, sizeof(connectMsg), 0) < 0) {
		printf("send failed with error %i\n", errno);
		close(sockFd);
		return 1;
	}

    printf("Connected to server. You can now send messages.\n");

	/* Loop */
	while (fgets(szInput, sizeof(szInput), stdin) != NULL) {
		if (strcmp(szInput, "exit\n") == 0) { /* If input is "exit" exit*/
			break;
		}
	
		int message_len = strlen(szInput) - 1; /* Størrelse minus newline */
		szInput[message_len] = '\0'; /* Legg til nullterminering */

		sendMessage.iMagicNumber = htonl(MAGIC_NUMBER);
		sendMessage.iMessageSize = htonl(sizeof(sendMessage) + message_len - sizeof(sendMessage.szMessage)); /* Send kun meldingen */

		/* Send metadata som header først */
		if (send(sockFd, &sendMessage, sizeof(sendMessage) - sizeof(sendMessage.szMessage), 0) < 0) {
			fprintf(stderr, "send failed with error %d\n", errno);
			close(sockFd);
			return 1;
		}
		/* Send meldingen  */
		if (send(sockFd, szInput, message_len, 0) < 0) {
			fprintf(stderr, "send failed with error %d\n", errno);
			break;
		}
	}

	/* Steng ned */
	close(sockFd); sockFd = -1;
	return 0;
}

