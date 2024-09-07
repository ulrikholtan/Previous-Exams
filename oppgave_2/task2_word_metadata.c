#include "task2_word_metadata.h"

void printMetadata(struct TASK2_WORD_METADATA word) {
	printf("Word:                       %s\n", word.szWord);
	printf("Palindrome:                 %s\n", word.bIsPalindrome ? "true" : "false");
	printf("Heterogram:                 %s\n", word.bIsHeterogram ? "true" : "false");
	printf("Uppercase:                  %s\n", word.bIsUppercase ? "true" : "false");
	printf("Lowercase:                  %s\n", word.bIsLowercase ? "true" : "false");
	printf("Anagram of another word:    %s\n", word.bIsAnagram ? "true" : "false");
	printf("Disjoint from another word: %s\n", word.bIsDisjoint ? "true" : "false");
	printf("Size:                       %d\n", word.iSize);
	printf("\n");
}


