#ifndef TASK2_WORD_METADATA_H
#define TASK2_WORD_METADATA_H

#include <stdio.h>

#define MAX_WORD_LENGTH 100

typedef enum {
    false = 0,
    true = 1
} bool;

struct TASK2_WORD_METADATA {
	int iIndex;
	bool bIsPalindrome;
	bool bIsHeterogram;
	bool bIsUppercase;
	bool bIsLowercase;
	bool bIsAnagram;
	bool bIsDisjoint;
	int iSize;
	char szWord[MAX_WORD_LENGTH];
};

void printMetadata(struct TASK2_WORD_METADATA word);

#endif /* TASK2_WORD_METADATA_H */

