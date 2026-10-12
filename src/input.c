#include "labtrack/input.h"

#include <stdio.h>

void clearInput(void) {
    int c; /* int, not char: getchar() returns EOF (-1) as an int */
    while ((c = getchar()) != '\n' && c != EOF);
}
 