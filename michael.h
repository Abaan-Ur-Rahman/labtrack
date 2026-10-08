#ifndef MICHAEL_H
#define MICHAEL_H

#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    char category[50];
    int quantity;
    char location[50];
} equipment_2;

void availability();
void reqborrow(const char *user);
void chk(const char *user);
void viewhistory(const char *user);

void studentMenu() {
    char user[50];
    int choice;

    printf("Enter your name: ");
    scanf("%s", user);

    do {
        printf("\n--- Student Menu ---\n");
        printf("1. View Equipment\n");
        printf("2. Request to Borrow\n");
        printf("3. Check Request Status\n");
        printf("4. View Borrowing History\n");
        printf("5. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

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

void availability() {
    equipment_2 e;
    FILE *f = fopen("equipment.txt", "r");
    if (!f) {
        printf("Cannot open equipment.txt\n");
        return;
    }

    printf("\nID | Name | Quantity | Category | Location\n");
    while (fscanf(f, "%d %s %s %d %s", &e.id, e.name, e.category, &e.quantity, e.location) == 5) {
        if (e.quantity > 0)
            printf("%d | %s | %s | %d | %s\n", e.id, e.name, e.category, e.quantity, e.location);
    }

    fclose(f);
}

void reqborrow(const char *user) {
    int id;
    printf("Enter Equipment ID: ");
    scanf("%d", &id);

    FILE *f = fopen("requests.txt", "a");
    if (!f) {
        printf("Cannot open requests.txt\n");
        return;
    }

    fprintf(f, "%s %d pending\n", user, id);
    fclose(f);
    printf("Request submitted.\n");
}

void chk(const char *user) {
    char name[50], status[20];
    int id, found = 0;
    FILE *f = fopen("requests.txt", "r");

    if (!f) {
        printf("Cannot open requests.txt\n");
        return;
    }

    printf("\nYour Requests:\n");
    while (fscanf(f, "%s %d %s", name, &id, status) == 3) {
        if (strcmp(name, user) == 0) {
            printf("ID: %d | Status: %s\n", id, status);
            found = 1;
        }
    }

    if (!found)
        printf("No requests found.\n");

    fclose(f);
}

void viewhistory(const char *user) {
    char name[50], date[20];
    int id, found = 0;
    FILE *f = fopen("borrow_history.txt", "r");

    if (!f) {
        printf("Cannot open borrow_history.txt\n");
        return;
    }

    printf("\nYour Borrowing History:\n");
    while (fscanf(f, "%s %d %s", name, &id, date) == 3) {
        if (strcmp(name, user) == 0) {
            printf("ID: %d | Date: %s\n", id, date);
            found = 1;
        }
    }

    if (!found)
        printf("No history found.\n");

    fclose(f);
}

#endif
