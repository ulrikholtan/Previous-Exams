#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Jeg endret navnet på filen palindrom til å ha endingen palindromE, 
da jeg syntes det var irriterende å jobbe med to ulike navn */
#include "include/ex2.h"
#include "include/palindrome.h"
#include "include/heterogram.h"
#include "include/all_upper.h"
#include "include/all_lower.h"
#include "include/anagram.h"
#include "include/disjoint.h"
#include "include/sanitizer.h"
#include "include/task2_word_metadata.h"

int main() {
	FILE *file;
	struct TASK2_WORD_METADATA words[MAX_WORD_LENGTH];
	char word[MAX_WORD_LENGTH];
	int numberOfWords = 0;

	/* Åpne fil i read modus */
	file = fopen("oppgave2.txt", "r");
	if (file == NULL) {
		perror("Error opening file");
		return 1;
	} else {
		printf("File read OK.\n");
	}

	/* Les ordene fra filen og lagre dem med verdier i struct */
	while (fgets(word, sizeof(word), file) != NULL && numberOfWords < MAX_WORD_LENGTH) {
		/* Remove characters ?*/
		sanitize(word);

		/* Fyll inn "metadata" */
		words[numberOfWords].iIndex = numberOfWords + 1;
		words[numberOfWords].bIsPalindrome = isPalindrome(word);
		words[numberOfWords].bIsHeterogram = isHeterogram(word);
		words[numberOfWords].bIsUppercase = isUpperCase(word);
		words[numberOfWords].bIsLowercase = isLowerCase(word);
		words[numberOfWords].bIsAnagram = false;
		words[numberOfWords].bIsDisjoint = false;
		words[numberOfWords].iSize = strlen(word);
		strcpy(words[numberOfWords].szWord, word);

		numberOfWords++;
	}

	/* Sjekk om ord er anagram eller disjoint */
	for (int i = 0; i < numberOfWords; i++) {
		for (int j = i + 1; j < numberOfWords; j++) {
			if (!words[i].bIsAnagram && isAnagram(words[i].szWord, words[j].szWord)) {
				words[i].bIsAnagram = true;
				words[j].bIsAnagram = true;
			}
			if (!words[i].bIsDisjoint &&!areDisjoint(words[i].szWord, words[j].szWord)) {
				words[i].bIsDisjoint = true;
				words[j].bIsDisjoint = true;
			}
		}
	
	}

	/* Husk å stenge filen */
	fclose(file);

	/* Skrive ut dataen */
	for (int i = 0; i < numberOfWords; i++) {
		printMetadata(words[i]);
	}


	/* Opprett en binær fil og skriv til den */
	file = fopen("utdata.bin", "wb");
	if (file == NULL) {
		perror("Error opening binary file");
		return 1;
	} else {
		printf("Binary file created OK. \n");
	}


	for (int i = 0; i < numberOfWords; i++) {
		/* Skriver structen til filen */
		fwrite(&words[i], sizeof(struct TASK2_WORD_METADATA), 1, file);
		/* Skriv ordet til filen */
		fwrite(words[i].szWord, sizeof(char), strlen(words[i].szWord) + 1, file);
		printf("Written %-2d word(s) to file\n", i + 1);
	}

	fclose(file);

	return 0;
}


