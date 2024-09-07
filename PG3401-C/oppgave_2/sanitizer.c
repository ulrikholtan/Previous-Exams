#include "sanitizer.h"
#include <ctype.h>
#include <string.h>

void sanitize(char *str) {
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (isalpha(str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';


}

