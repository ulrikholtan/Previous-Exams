#ifndef RESERVATION_SYSTEM_H
#define RESERVATION_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Struct for ordre */
typedef struct Order {
	char szGuestName[50];
	char szItemName[50];
	int iQuantity;
	int iPrice;
	struct Order *pNext;
} Order;

/* Struct for reservasjoner */
typedef struct Reservation {
	int iReservationNumber;
	int iTableNumber;
	char szBookingName[50];
	int iSeats;
	int iTime;
	struct Reservation *pPrev;
	struct Reservation *pNext;
	Order *pOrders;
} Reservation;

/* Funksjon prototyper */
char* formatDateTime(time_t epochTime);
Order *createNewOrder(const char *szGuestName, const char *szItemName, int iQuantity, int iPrice);
Reservation *createNewReservation(int iTableNumber, const char *szBookingName, int iSeats, Reservation **ppHead);
void createReservation(Reservation **ppHead);
void printAllReservations(Reservation *pHead);
void printReservation(Reservation *pHead);
void printReservationByName(Reservation *pHead);
void deleteReservationByName(Reservation **ppHead);
void createOrder(Reservation *pHead);
void printOrdersForTable(Reservation *pHead);
void printOrdersForGuestAtTable(Reservation *pHead);
void freeOrderList(Order *pOrder);
void freeReservationList(Reservation **ppHead);
void endProgram(Reservation **ppHead);

#endif /* RESERVATION_SYSTEM_H */

