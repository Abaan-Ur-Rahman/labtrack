#include "labtrack/input.h"

#include <stdio.h>

void clearInput(void) {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}
