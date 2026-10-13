#ifndef LABTRACK_MODELS_H
#define LABTRACK_MODELS_H

/* Data types shared by every module. */

/* Size of each text field, including the terminating '\0'.
 * File reads must use a width of TEXT_LEN - 1, e.g. "%49s". */
#define TEXT_LEN 50

/* Largest equipment ID and largest quantity of one item the system accepts. */
#define MAX_ID 999999
#define MAX_QUANTITY 100000

/* One equipment record, as stored in equipment.txt:
 * id name category quantity location */
typedef struct {
    int id;
    char name[TEXT_LEN];
    char category[TEXT_LEN];
    int quantity;
    char location[TEXT_LEN];
} Equipment;

#endif /* LABTRACK_MODELS_H */