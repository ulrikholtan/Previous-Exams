#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "include/ex6.h"

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Incorrect syntax. Use the program like this: %s <filename>\n", argv[0]);
		return 1;
	}

	const char* pszInputFileName = argv[1];
	char* pszDot = strrchr(pszInputFileName, '.');
	if (pszDot == NULL) {
		printf("Error: No file extension found.\n");
		return 1;
	}

	size_t baseFileNameLength = pszDot - pszInputFileName;
	size_t outputFileNameLength = baseFileNameLength + strlen("_beautified.c") + 1;
	char* pszOutputFileName = (char*)malloc(outputFileNameLength);
	if (pszOutputFileName == NULL) {
		printf("Memory allocation failed.\n");
		return 1;
	}

	strncpy(pszOutputFileName, pszInputFileName, baseFileNameLength);
	pszOutputFileName[baseFileNameLength] = '\0';
	strcat(pszOutputFileName, "_beautified.c");

	beautify(pszInputFileName, pszOutputFileName);

	free(pszOutputFileName);

	return 0;
}

void replaceSpacesWithTabs(char* pszLine) {
	char szBuffer[BUFFER_SIZE] = {0};
	int iBufferIndex = 0;
	for (int i = 0; pszLine[i] != '\0'; ++i) {
		if (pszLine[i] == ' ' && pszLine[i+1] == ' ' && pszLine[i+2] == ' ') {
			szBuffer[iBufferIndex++] = '\t'; /* Hvis det er 3 forekomster av mellomrom, bytt ut med tabulator */
			i += 2; /* Skip de neste to */
		} else {
			szBuffer[iBufferIndex++] = pszLine[i];
		}
	}
	szBuffer[iBufferIndex] = '\0';
	strcpy(pszLine, szBuffer); /* Kopier nytt innhold inn i original */
}

void addHungarianNotation(char* pszLine) {
	char szBuffer[BUFFER_SIZE] = {0};
	char* pszToken = strtok(pszLine, ";");

	while (pszToken != NULL) {
		char* pszCharPointer = strstr(pszToken, "char *");
		if (pszCharPointer != NULL) {
			char* pszVarStart = pszCharPointer + 6; /* Gå forbi "char *" */
			/* Hopp over mellonrom etter "char *"*/
			while (isspace((unsigned char)*pszVarStart)) {
				pszVarStart++;
			}
			int iNameLength = strcspn(pszVarStart, " ,;\t\n"); /* Finn slutten av variabel */
			strncpy(szBuffer, pszToken, pszVarStart - pszToken);
			szBuffer[pszVarStart - pszToken] = '\0';
			strcat(szBuffer, "psz");
			strncat(szBuffer, pszVarStart, iNameLength);
			strcat(szBuffer, pszVarStart + iNameLength);
		} else {
			strcat(szBuffer, pszToken);
		}
		pszToken = strtok(NULL, ";");
		if (pszToken != NULL) strcat(szBuffer, ";");
	}
	strcpy(pszLine, szBuffer);
}

char* extractVariableName(char* pszDeclaration) {
	char* pszVarName = strstr(pszDeclaration, "int ");
	if (pszVarName) {
		pszVarName += 4; /* Ta variabel navn etter type "int " */
		while (*pszVarName && isspace(*pszVarName)) { /* Ikke inkluder whitespace */
			pszVarName++;
		}
		char* pszEnd = pszVarName;
		while (*pszEnd && !isspace(*pszEnd) && *pszEnd != ';') {
			pszEnd++;
		}
		*pszEnd = '\0';
	}
	return pszVarName;
}

int isIntDeclaration(const char *pszLine) {
	/* Hopp over whitespace før deklarering*/
	while (*pszLine != '\0' && isspace((unsigned char)*pszLine)) { 
		pszLine++;
	}
	return strncmp(pszLine, "int ", 4) == 0; /* Let etter "int " etter indentering*/

}

char* extractLeadingWhitespace(const char *pszLine) {
	static char szWhitespace[BUFFER_SIZE];
	int i = 0;
	while (isspace((unsigned char)pszLine[i])) {
		szWhitespace[i] = pszLine[i];
		i++;
	}
	szWhitespace[i] = '\0';
	return szWhitespace;
}

int containsWhile(char *pszLine) {
	return strstr(pszLine, "while") != NULL;
}


void convertWhileLoop(char *szLastIntDeclaration, char *pszWhileLine, FILE *pFileOutput) {
	char szCondition[256] = {0};
	char *pszConditionStart = strchr(pszWhileLine, '(') + 1;
	char *pszConditionEnd = strrchr(pszWhileLine, ')');

	if (pszConditionStart && pszConditionEnd) {
		strncpy(szCondition, pszConditionStart, pszConditionEnd - pszConditionStart);
	}

	char szVariableName[256];
	/* Kopier eksisterende condition */
	strcpy(szVariableName, extractVariableName(szLastIntDeclaration)); 

	/* Få med indenteringen før en eventuell loop */
	char* pszLeadingWhitespace = extractLeadingWhitespace(pszWhileLine);
	
	fprintf(pFileOutput, "%sfor (%s; %s; %s++) {\n", pszLeadingWhitespace, szLastIntDeclaration, szCondition, szVariableName);
}

void beautify(const char* pszInputFileName, const char* pszOutputFileName) {
	FILE* pFileInput = fopen(pszInputFileName, "r");
	if (!pFileInput) {
		printf("Error opening input file\n");
		return;
	}

	FILE* pFileOutput = fopen(pszOutputFileName, "w");
	if (!pFileOutput) {
		printf("Error creating output file.\n");
		fclose(pFileInput);
		return;
	}

	char szLine[BUFFER_SIZE];
	char szPrevLine[BUFFER_SIZE] = {0}; /* Følger med på forrige linje som ikke var whitespace */
	char szLastIntDeclaration[BUFFER_SIZE] = {0}; /* Lagrer også forrige deklarering av variabel */
	int bLastLineWasDeclaration = FALSE; /* Flagg for å sjekke om siste linje var deklarering */

	while (fgets(szLine, sizeof(szLine), pFileInput)) {
		char szOriginalLine[BUFFER_SIZE];
		strcpy(szOriginalLine, szLine); /* Husk den orginale linjen */

		replaceSpacesWithTabs(szLine);  /* Gjør om spaces til tabs */
		addHungarianNotation(szLine);   /* Gjør om variabel til Hungarian notation*/

		if (isIntDeclaration(szLine)) { /* Sjekk om en linje er en int deklarering */
			strcpy(szLastIntDeclaration, szLine);
			bLastLineWasDeclaration = TRUE; /* Toggle flagg */
			continue;
		}

		if (containsWhile(szLine)) {
			convertWhileLoop(szLastIntDeclaration, szLine, pFileOutput);
			bLastLineWasDeclaration = FALSE; /* Reset flagg */
			continue;
		}

		/* Hvis siste linje ikke var en loop, men forrige er en deklarering må vi printe variabelen */
		if (bLastLineWasDeclaration) {
			fputs(szLastIntDeclaration, pFileOutput);
			bLastLineWasDeclaration = FALSE; /* Reset flagg*/
		}

/* Vi har allerede behandlet inkrementering av variabel, så når vi finner denne i loop skipper vi */
		if (strstr(szLine, "++") != NULL) {
			continue;
		}

		fputs(szLine, pFileOutput); /* skriv linjen hvis det ikke er noe convertering */
		strcpy(szPrevLine, szOriginalLine); /* Oppdater forrige linje */
	}

	fclose(pFileInput);
	fclose(pFileOutput);
	printf("Beautification complete. Output saved in %s.\n", pszOutputFileName);
}

