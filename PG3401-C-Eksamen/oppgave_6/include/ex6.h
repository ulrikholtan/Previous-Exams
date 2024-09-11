#ifndef BEAUTIFY_H
#define BEAUTIFY_H

#include <stdio.h>

/* Bytt ut space indentering med tabulator */
void replaceSpacesWithTabs(char *pszLine);

/* Gjør om variabel til Hungarian notation */
void addHungarianNotation(char *pszLine);

/* Henter ut variabel navnet i en deklarering */
char* extractVariableName(char* pszDeclaration);

/* Sjekk om linjen deklarerer en variabel */
int isIntDeclaration(const char *pszLine);

/* Kopier indenteringen før en linje */
char* extractLeadingWhitespace(const char *pszLine);

/* Sjekk om linjen er en "while loop" */
int containsWhile(char *pszLine);

/* Gjør om en while-loop til en for-loop */
void convertWhileLoop(char *pszPrevDeclaration, char *pszWhileLine, FILE *pFileOutput);

/* Hovedfunksjon for behandling av filer */
void beautify(const char *pszInputFileName, const char *pszOutputFileName);

#endif /* BEAUTIFY_H */
