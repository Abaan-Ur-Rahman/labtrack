#include "labtrack/student.h"
#include "labtrack/input.h"
#include "labtrack/models.h"

#include <stdio.h>
#include <string.h>

static void availability(void);
static void reqborrow(const char *user);
static void chk(const char *user);
static void viewhistory(const char *user);

void studentMenu(void) {
    char user[TEXT_LEN];
    int choice;

    read_word("Enter your name: ", user, sizeof user);

    do {
        printf("\n--- Student Menu ---\n");
        printf("1. View Equipment\n");
        printf("2. Request to Borrow\n");
        printf("3. Check Request Status\n");
        printf("4. View Borrowing History\n");
        printf("5. Exit\n");
        choice = read_int("Choice: ", 1, 5);

        switch (choice) {
            case 1: availability(); break;
            case 2: reqborrow(user); break;
            case 3: chk(user); break;
            case 4: viewhistory(user); break;
            case 5: printf("Goodbye.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
}

static void availability(void) {
    Equipment e;
    FILE *f = fopen("equipment.txt", "r");
    if (!f) {
        printf("Cannot open equipment.txt\n");
        return;
    }

    printf("\nID | Name | Quantity | Category | Location\n");
    while (fscanf(f, "%d %49s %49s %d %49s", &e.id, e.name, e.category, &e.quantity, e.location) == 5) {
        if (e.quantity > 0)
            printf("%d | %s | %s | %d | %s\n", e.id, e.name, e.category, e.quantity, e.location);
    }

    fclose(f);
}

static void reqborrow(const char *user) {
    int id = read_int("Enter Equipment ID: ", 1, MAX_ID);

    FILE *f = fopen("requests.txt", "a");
    if (!f) {
        printf("Cannot open requests.txt\n");
        return;
    }

    fprintf(f, "%s %d pending\n", user, id);
    fclose(f);
    printf("Request submitted.\n");
}

static void chk(const char *user) {
    char name[TEXT_LEN], status[20];
    int id, found = 0;
    FILE *f = fopen("requests.txt", "r");

    if (!f) {
        printf("Cannot open requests.txt\n");
        return;
    }

    printf("\nYour Requests:\n");
    while (fscanf(f, "%49s %d %19s", name, &id, status) == 3) {
        if (strcmp(name, user) == 0) {
            printf("ID: %d | Status: %s\n", id, status);
            found = 1;
        }
    }

    if (!found)
        printf("No requests found.\n");

    fclose(f);
}

static void viewhistory(const char *user) {
    char name[TEXT_LEN], date[20];
    int id, found = 0;
    FILE *f = fopen("borrow_history.txt", "r");

    if (!f) {
        printf("Cannot open borrow_history.txt\n");
        return;
    }

    printf("\nYour Borrowing History:\n");
    while (fscanf(f, "%49s %d %19s", name, &id, date) == 3) {
        if (strcmp(name, user) == 0) {
            printf("ID: %d | Date: %s\n", id, date);
            found = 1;
        }
    }

    if (!found)
        printf("No history found.\n");

    fclose(f);
}