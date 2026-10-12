#ifndef LABTRACK_INPUT_H
#define LABTRACK_INPUT_H
 
#include <stddef.h>
 
/* Helpers for reading user input from the keyboard.
 *
 * Every read takes a whole line, so a bad answer can never be left behind
 * in the input buffer, and every read is bounded by the size of the buffer
 * it writes into. If input ends (Ctrl+Z on Windows, Ctrl+D on Linux), the
 * program exits instead of looping forever. */
 
/* Converts text to an int between min and max (inclusive).
 * Surrounding spaces are allowed; anything else ("abc", "12abc") is not.
 * Returns 1 and stores the number in *out, or returns 0 if text is invalid. */
int parse_int(const char *text, int min, int max, int *out);
 
/* Reads one line into buf (size bytes, including the '\0'), without the
 * newline. Returns 1 on success, or 0 if the line did not fit; the rest of
 * that line is then discarded. Exits the program if input has ended. */
int read_line(char *buf, size_t size);
 
/* Shows prompt until the user types a whole number from min to max. */
int read_int(const char *prompt, int min, int max);
 
/* Shows prompt until the user types one non-empty word that fits in buf.
 * Spaces are refused because the data files separate fields with spaces. */
void read_word(const char *prompt, char *buf, size_t size);
 
/* Discards everything left on the current input line, including the '\n'. */
void clearInput(void);
 
#endif /* LABTRACK_INPUT_H */