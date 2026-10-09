#ifndef LABTRACK_ADMIN_H
#define LABTRACK_ADMIN_H

/* Admin role: add, view, remove and update equipment. */

/* Compares two strings; returns 1 if equal. */
int string_equal(char *a, char *b);

/* Shows the admin menu until the user logs out. */
void admin_main(void);

#endif /* LABTRACK_ADMIN_H */
