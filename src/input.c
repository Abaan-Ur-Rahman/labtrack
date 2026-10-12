#include "labtrack/input.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_int(const char *text, int min, int max, int *out) {
    char *end;
    long value;

    errno = 0;
    value = strtol(text, &end, 10);
    if (end == text || errno == ERANGE) {
        return 0; /* no digits at all, or too big for a long */
    }
    while (isspace((unsigned char)*end)) {
        end++;
    }
    if (*end != '\0' || value < min || value > max) {
        return 0; /* trailing junk such as "12abc", or out of range */
    }
    *out = (int)value;
    return 1;
}

int read_line(char *buf, size_t size) {
    if (fgets(buf, (int)size, stdin) == NULL) {
        printf("\nInput ended. Exiting.\n");
        exit(EXIT_SUCCESS);
    }
    if (strchr(buf, '\n') == NULL && !feof(stdin)) {
        /* No newline in buf: either the line exactly filled it, or it was
         * longer. Peek at the next character to tell which. */
        int next = getchar();
        if (next != '\n' && next != EOF) {
            clearInput(); /* too long: throw away the rest of the line */
            return 0;
        }
    }
    buf[strcspn(buf, "\r\n")] = '\0';
    return 1;
}

int read_int(const char *prompt, int min, int max) {
    char line[32];
    int value;

    for (;;) {
        printf("%s", prompt);
        fflush(stdout);
        if (read_line(line, sizeof line) && parse_int(line, min, max, &value)) {
            return value;
        }
        printf("Please enter a whole number from %d to %d.\n", min, max);
    }
}

void read_word(const char *prompt, char *buf, size_t size) {
    for (;;) {
        printf("%s", prompt);
        fflush(stdout);
        if (!read_line(buf, size)) {
            printf("Too long: use at most %d characters.\n", (int)(size - 1));
        } else if (buf[0] == '\0') {
            printf("Please type something.\n");
        } else if (strpbrk(buf, " \t") != NULL) {
            printf("No spaces, please (use _ instead).\n");
        } else {
            return;
        }
    }
}

void clearInput(void) {
    int c; /* int, not char: getchar() returns EOF (-1) as an int */
    while ((c = getchar()) != '\n' && c != EOF);
}
