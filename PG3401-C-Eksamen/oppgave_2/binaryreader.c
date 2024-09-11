#include <stdio.h>
#include <stdlib.h>

#include "include/binaryreader.h"
#include "include/task2_word_metadata.h"

#define MAX_WORD_LENGTH 100

#define TRUE 1
#define FALSE 0

int main(int iArgc, char *cArgv[]) {
	if (iArgc != 2) {
		printf("Syntax: ./binaryreader <filnavn.bin>\n");
		return 1;
	}

	FILE *binaryFile;
	struct TASK2_WORD_METADATA wordMetadata;
	char word[MAX_WORD_LENGTH];
	int numberOfWords = 0;

	/* Åpne filen i read mode */
	binaryFile = fopen(cArgv[1], "rb");
	if (binaryFile == NULL) {
		perror("Error opening binary file");
		return 1;
	} else {
		printf("Binary file opened OK.\n");
	}

	/* Les hvert ord og print i leselig format */
	while (fread(&wordMetadata, sizeof(struct TASK2_WORD_METADATA), 1, binaryFile) == 1) {
		fread(word, sizeof(char), wordMetadata.iSize + 1, binaryFile);

		/* Print metadataen og ordet */
		printf("Index:      %d\n", wordMetadata.iIndex);
		printf("Palindrome: %s\n", wordMetadata.bIsPalindrome ? "true" : "false");
		printf("Heterogram: %s\n", wordMetadata.bIsHeterogram ? "true" : "false");
		printf("Uppercase:  %s\n", wordMetadata.bIsUppercase ? "true" : "false");
		printf("Lowercase:  %s\n", wordMetadata.bIsLowercase ? "true" : "false");
		printf("Anagram:    %s\n", wordMetadata.bIsAnagram ? "true" : "false");
		printf("Disjoint:   %s\n", wordMetadata.bIsDisjoint ? "true" : "false");
		printf("Size:       %d\n", wordMetadata.iSize);
		printf("Word:       %s\n", word);
		printf("\n");

		numberOfWords++;
	}

	/* steng filen */
	fclose(binaryFile);

	printf("Read %d words from binary file.\n", numberOfWords);

	return 0;
}


