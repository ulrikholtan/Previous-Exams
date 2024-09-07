#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/ex3.h"

int main() {
	Reservation *pHead = NULL;
	char szInput[100];

	while (1) {
		printf("\nVelg funksjon.\n");
		printf("1. Lag reservasjon\n");
		printf("2. Hent reservasjonsnummer.\n");
		printf("3. Finn reservasjon på navn.\n");
		printf("4. Slett reservasjon.\n");
		printf("5. Legg til bestilling for bord.\n");
		printf("6. Print ordre for bord.\n");
		printf("7. Print ordre for person.\n");
		printf("8. Hent alle reservasjoner.\n");
		printf("9. Avslutt\n");
		printf("\n");
		printf("Valg: ");
	
		fgets(szInput, sizeof(szInput), stdin);
		int option = atoi(szInput);

		switch (option) {
			case 1:
				createReservation(&pHead);
				break;
			case 2: {
				printReservation(pHead);
				break;
			}
			case 3: {
				printReservationByName(pHead);
				break;
			}
			case 4:
				deleteReservationByName(&pHead);
				break;
			case 5:
				createOrder(pHead);
				break;
			case 6: {
				printOrdersForTable(pHead);
				break;
			}
			case 7:
				printOrdersForGuestAtTable(pHead);
				break;
			case 8:
				printAllReservations(pHead);
				break;
			case 9:
				endProgram(&pHead);
				break;
			default:
				printf("Ugyldig valg, vennligst prøv igjen.\n");
		}

	}
	return 0;
}

/* Hjelpe funksjon for å konvertere tid fra UNIX Epoch til GMT+1 */
char* formatDateTime(time_t epochTime) {
	struct tm *localTimeInfo = localtime(&epochTime);
	static char formattedDateTime[80];
	strftime(formattedDateTime, sizeof(formattedDateTime), "%d.%m.%Y %H:%M", localTimeInfo);
	return formattedDateTime;
}

/* Lag en ny ordre */
Order *createNewOrder(const char *szGuestName, const char *szItemName, int iQuantity, int iPrice) {
	Order *pNewOrder = (Order *)malloc(sizeof(Order));
	if (pNewOrder == NULL) {
		fprintf(stderr, "Minne allokering feilet for ordre\n");
		exit(1);
	}
	strcpy(pNewOrder->szGuestName, szGuestName);
	strcpy(pNewOrder->szItemName, szItemName);
	pNewOrder->iQuantity = iQuantity;
	pNewOrder->iPrice = iPrice;
	pNewOrder->pNext = NULL;
	return pNewOrder;
}

/* Lag en ny reservasjon og plasser den i en sortert liste */
Reservation *createNewReservation(int iTableNumber, const char *szBookingName, int iSeats, Reservation **ppHead) {
	Reservation *pNewReservation = (Reservation *)malloc(sizeof(Reservation));
	if (pNewReservation == NULL) {
		fprintf(stderr, "Minne allokering feilet for reservasjon\n");
		exit(1);
	}
	pNewReservation->iTableNumber = iTableNumber;
	strcpy(pNewReservation->szBookingName, szBookingName);
	pNewReservation->iSeats = iSeats;

	/* Regn ut tiden for reservasjonen */
	time_t currentTime;
	time(&currentTime);
	pNewReservation->iTime = (int)currentTime;

	pNewReservation->pNext = NULL;
	pNewReservation->pPrev = NULL;
	pNewReservation->pOrders = NULL; /* Her kommer ordrene som linked list */

	/* Regn ut reservasjonsnummeret basert på posisjon i listen */
	if (*ppHead == NULL || strcmp((*ppHead)->szBookingName, szBookingName) >= 0) {
		/* Sett inn på begynnelsen*/
		pNewReservation->iReservationNumber = 1;
		pNewReservation->pNext = *ppHead;
		if (*ppHead != NULL) {
			(*ppHead)->pPrev = pNewReservation;
			/* Oppdater reservasjonsnummer */
			Reservation *temp = *ppHead;
			while (temp != NULL) {
				temp->iReservationNumber++;
				temp = temp->pNext;
			}
		}
		*ppHead = pNewReservation;
	} else {
		Reservation *pThis = *ppHead;
		int reservationNumber = 1;
		while (pThis->pNext != NULL && strcmp(pThis->pNext->szBookingName, szBookingName) < 0) {
			pThis = pThis->pNext;
			reservationNumber++;
		}
		/* Sett inn etter this */
		pNewReservation->iReservationNumber = reservationNumber + 1;
		pNewReservation->pNext = pThis->pNext;
		pNewReservation->pPrev = pThis;
		if (pThis->pNext != NULL) {
			pThis->pNext->pPrev = pNewReservation;
		}
		pThis->pNext = pNewReservation;
		pNewReservation->pPrev = pThis;

		/* Oppdater reservasjonsnummer for resten av listen */
		Reservation *temp = pThis;
		while (temp->pNext != NULL) {
			temp->pNext->iReservationNumber++;
			temp = temp->pNext;
		}
	}
	return pNewReservation;
}

/* FUNKSJONER FOR RESERVASJONER */
/* Oppretting av reservasjon */
void createReservation(Reservation **ppHead) {
	int iTableNumber, iSeats;
	char szBookingName[50] = {0};
	char szInput[100];
	char *pEndPtr;

	while (1) {
		printf("Skriv inn bordnummer: ");
		fgets(szInput, sizeof(szInput), stdin);
		szInput[strcspn(szInput, "\n")] = '\0';
		iTableNumber = strtol(szInput, &pEndPtr, 10);
		if (*pEndPtr == '\0' && pEndPtr != szInput) {
			/**/
			break;
		} else {
			printf("Ugyldig bordnummer. Venligst oppgi et tall.\n");
		}
	}


	iTableNumber = atoi(szInput);

	printf("Skriv inn navn for bookingen: ");
	fgets(szBookingName, sizeof(szBookingName), stdin);
	szBookingName[strcspn(szBookingName, "\n")] = '\0';

	printf("Skriv inn antall gjester: ");
	fgets(szInput, sizeof(szInput), stdin);
	iSeats = atoi(szInput);

	Reservation *pNewReservation = createNewReservation(iTableNumber, szBookingName, iSeats, ppHead);
	if (pNewReservation != NULL) {
		printf("\nReservasjon opprettet for bord #%d.\n", iTableNumber);
	} else {
		printf("\nFeil ved opprettelse av reservasjon.\n");
	}
}
/* Skriv ut alle reservasjoner */
void printAllReservations(Reservation *pHead) {
	Reservation *pReservation = pHead;
	while (pReservation != NULL) {
		printf("\nReservasjonsnummer: %d.\n", pReservation->iReservationNumber);
		printf("Bordnummer: %d.\n", pReservation->iTableNumber);
		printf("Booking navn: %s.\n", pReservation->szBookingName);
		printf("Gjester: %d.\n", pReservation->iSeats);
		printf("Dato og klokkeslett: %s\n", formatDateTime((time_t)pReservation->iTime));

		pReservation = pReservation->pNext;
	}
}

/* Skriv ut reservasjon basert på reservasjonsnummer N*/
/* Obs. Reservasjonssnummer er dynamisk utifra alfabetisk rekkefølge */
void printReservation(Reservation *pHead) {
	char szInput[100];
	int iReservationNumber;
	printf("Skriv inn reservasjonsnummer: ");
	fgets(szInput, sizeof(szInput), stdin);
	iReservationNumber = atoi(szInput);

	Reservation *pReservation = pHead;
	while (pReservation != NULL) {
		if (pReservation->iReservationNumber == iReservationNumber) {
			printf("\nReservasjonsnummer: %d\n", pReservation->iReservationNumber);
			printf("Bordnummer: %d\n", pReservation->iTableNumber);
			printf("Booking navn: %s\n", pReservation->szBookingName);
			printf("Gjester: %d\n", pReservation->iSeats);

			/* Regn om klokkeslettet til GMT+1 */
			time_t reservationEpochTime = (time_t)pReservation->iTime;
			struct tm *localTimeInfo = localtime(&reservationEpochTime);
			char formattedDateTime[80];
			strftime(formattedDateTime, sizeof(formattedDateTime), "%d.%m.%Y %H:%M",localTimeInfo);

			printf("Dato og klokkeslett: %s\n", formattedDateTime);

			printf("Bestillinger:\n");
			Order *pOrder = pReservation->pOrders;
			int totalPrice = 0;
			while (pOrder != NULL) {
				printf("Gjest: %s, Vare: %s, Antall: %d, Pris: %d\n", pOrder->szGuestName, pOrder->szItemName, pOrder->iQuantity, pOrder->iPrice);
				totalPrice += pOrder->iQuantity * pOrder->iPrice;
				pOrder = pOrder->pNext;
			}
			printf("Totalpris: %d\n", totalPrice);
			return;
		}
		pReservation = pReservation->pNext;
	}
	printf("\nReservasjon med nummer %d ikke funnet.\n", iReservationNumber);
}

/* Print reservasjon for gitt navn */
void printReservationByName(Reservation *pHead) {
	char bookingName[50];
	printf("Skriv inn navn for bookingen: ");
	fgets(bookingName, sizeof(bookingName), stdin);
	bookingName[strcspn(bookingName, "\n")] = '\0';	

	Reservation *pReservation = pHead;
	int booleanFound = 0;

	while (pReservation != NULL) {
		/* strcasecmp er ikke case-sensitive, og finner navn uavhengig av kapitalisering */
		if (strcasecmp(pReservation->szBookingName, bookingName) == 0) {
			booleanFound = 1;
			printf("\nBooking Navn: %s\n", pReservation->szBookingName);
			printf("Bord nummer: %d\n", pReservation->iTableNumber);
			printf("Dato og klokkeslett: %s\n", formatDateTime((time_t)pReservation->iTime));
		}
		pReservation = pReservation->pNext;
	}
	if (!booleanFound) {
		printf("\nIngen reservasjoner på navnet: %s\n", bookingName);
	}
}

/* Slett reservasjonen for et gitt navn */
void deleteReservationByName(Reservation **ppHead) {
	char bookingName[50];
	printf("Skriv inn navn for bookingen som skal slettes: ");
	fgets(bookingName, sizeof(bookingName), stdin);
	bookingName[strcspn(bookingName, "\n")] = '\0';

	Reservation *prevReservation = NULL;
	Reservation *thisReservation = *ppHead;

	/* Gå gjennom listen for å lete etter matchende navn */
	while (thisReservation != NULL && strcmp(thisReservation->szBookingName, bookingName) != 0) {
		prevReservation = thisReservation;
		thisReservation = thisReservation->pNext;
	}

	/* Funnet matchende navn */
	if (thisReservation != NULL) {
		if (prevReservation == NULL) {
			*ppHead = thisReservation->pNext;
		} else {
			prevReservation->pNext = thisReservation->pNext;
		}

		/* Husk å frigjør minne for ordrene til reservasjonen ! */
		freeOrderList(thisReservation->pOrders);
		/* Frigjør selve reservasjonen */
		free(thisReservation);
		printf("\nReservasjonen for navnet \"%s\" er slettet.\n", bookingName);
	} else {
		printf("\nIngen reservasjon funnet for navnet \"%s\".\n", bookingName);
	}
}

/* FUNKSJONER FOR ORDRE */
/* Oppretting av en ordre */
void createOrder(Reservation *pHead) {
	char szGuestName[50], szItemName[50], szInput[100];
	int iTableNumber;
	int iQuantity;
	int iPrice;

	printf("Skriv inn bordnummer: ");
	fgets(szInput, sizeof(szInput), stdin);
	iTableNumber = atoi(szInput);

	/* Finn reservasjonen for bordnummeret */
	Reservation *pReservation = pHead;
	while (pReservation != NULL && pReservation->iTableNumber != iTableNumber) {
		pReservation = pReservation->pNext;
	}

	if (pReservation == NULL) {
		printf("Bordnummeret ble ikke funnet.\n");
		return;
	}

	printf("Skriv inn navn på gjest: ");
	fgets(szGuestName, sizeof(szGuestName), stdin);
	szGuestName[strcspn(szGuestName, "\n")] = '\0';

	printf("Skriv inn varenavn: ");
	fgets(szItemName, sizeof(szItemName), stdin);
	szItemName[strcspn(szItemName, "\n")] = '\0';

	printf("Skriv inn pris: ");
	fgets(szInput, sizeof(szInput), stdin);
	iPrice = atoi(szInput);
	
	printf("Skriv inn antall: ");
	fgets(szInput, sizeof(szInput), stdin);
	iQuantity = atoi(szInput);

	/* Finn siste ordre */
	Order *pLastOrder = pReservation->pOrders;
	while (pLastOrder != NULL && pLastOrder->pNext != NULL) {
		pLastOrder = pLastOrder->pNext;
	}

	/* Lag ny ordre */
	Order *pNewOrder = createNewOrder(szGuestName, szItemName, iQuantity, iPrice);
	if (pNewOrder != NULL) {
		printf("\nOrdre opprettet.\n");
		/* Link ordren med den eksisterende ordren */
		if (pLastOrder == NULL) {
			pReservation->pOrders = pNewOrder;
		} else {
			while (pLastOrder->pNext != NULL) {
				pLastOrder = pLastOrder->pNext;
			}
			pLastOrder->pNext = pNewOrder;
		}
	} else {
		printf("\nOrdre feilet.\n");
	}
}

/* Skriv ut alle ordre som er gjort for et gitt bord */
void printOrdersForTable(Reservation *pHead) {
	char szInput[10];
	int iTableNumber;
	printf("Skriv inn bordnummer: ");
	fgets(szInput, sizeof(szInput), stdin);
	iTableNumber = atoi(szInput);

	Reservation *pReservation = pHead;
	int totalPrice = 0; /* Totalprisen for border starter på 0 */

	while (pReservation != NULL) {
		if (pReservation->iTableNumber == iTableNumber) {
			printf("\nOrdre for bord #%d:\n", iTableNumber);
			Order *pOrder = pReservation->pOrders;
			while (pOrder != NULL) {
				printf("Navn: %s, Vare: %s, Antall: %d, Pris: %d\n", pOrder->szGuestName, pOrder->szItemName, pOrder->iQuantity, pOrder->iPrice);
				totalPrice += (pOrder->iQuantity * pOrder->iPrice);
				pOrder = pOrder->pNext;
			}
			printf("Total pris: %d\n", totalPrice);
			return; /* Fant bordet */
		}
		pReservation = pReservation->pNext;
	}
	printf("\nIngen ordre funnet for bord %d\n", iTableNumber);
}

/* Skriv ut ordre for en gitt gjest ved et bord (Separat regning) */
void printOrdersForGuestAtTable(Reservation *pHead) {
	int iTableNumber;
	char cGuestName[50];
	char cInput[100];

	printf("Skriv inn bordnummer: ");
	fgets(cInput, sizeof(cInput), stdin);
	iTableNumber = atoi(cInput);

	printf("Skriv inn gjestens navn: ");
	fgets(cGuestName, sizeof(cGuestName), stdin);
	cGuestName[strcspn(cGuestName, "\n")] = '\0';

	Reservation *pReservation = pHead;
	int booleanFound = 0;
	int iTotalSum = 0;

	while (pReservation != NULL) {
		if (pReservation->iTableNumber == iTableNumber) {
			printf("\nOrdre for bord #%d:\n", iTableNumber);
			Order *pOrder = pReservation->pOrders;
			while (pOrder != NULL) {
				if (strcmp(pOrder->szGuestName, cGuestName) == 0) {
					printf("Navn: %s, Vare: %s, Antall: %d, Pris: %d\n", pOrder->szGuestName, pOrder->szItemName, pOrder->iQuantity, pOrder->iPrice);
					iTotalSum += pOrder->iQuantity * pOrder->iPrice;
					booleanFound = 1;
				}
				pOrder = pOrder->pNext;
			}
			if (!booleanFound) {
				printf("\nIngen ordre funnet for gjest %s på bord %d\n", cGuestName, iTableNumber);
			} else {
				printf("Total sum: %d\n", iTotalSum);
			}
			return;
		}
		pReservation = pReservation->pNext;
	}
	printf("Ingen ordre funnet for bord %d\n", iTableNumber);
}

/* Funksjon for å frigjøre ordre som er gjort*/
void freeOrderList(Order *pOrder) {
	while (pOrder != NULL) {
		Order *pNextOrder = pOrder->pNext;
		free(pOrder);
		pOrder = pNextOrder;
	}
}

/* Funksjon for å frigjøre en reservasjon, med eventuelle tilhørende ordre */
void freeReservationList(Reservation **ppHead) {
	Reservation *pThis = *ppHead;
	while (pThis != NULL) {
		Reservation *pNext = pThis->pNext;
		/* Frigjør hver ordre i listen over ordre*/
		freeOrderList(pThis->pOrders);
		free(pThis);
		pThis = pNext;
	}
	*ppHead = NULL;
}

/* Funksjon for å avslutte program, og frigjøre minnet */
void endProgram(Reservation **ppHead) {
	printf("Avslutter program\n");
	freeReservationList(ppHead);
	exit(0);
}
