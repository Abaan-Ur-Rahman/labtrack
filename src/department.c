#include "labtrack/department.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Equipment_3 {
    char id[10];
    char name[50];
    int quantity;
    char location[30];
    char category[30];
};

struct BorrowLog {
    char studentId[15];
    char equipmentId[10];
    char borrowDate[15];
    int returned;
};

struct Request {
    char studentId[15];
    char equipmentId[10];
    char requestDate[15];
    char status[10];
};

static void displayEquipments(void) {
    FILE *file;
    struct Equipment_3 item;

    file = fopen("equipment.txt", "r");
    if (file == NULL) {
        printf("Could not open equipment file.\n");
        return;
    }

    printf("\nList of Equipment:\n");
    while (fscanf(file, "%s %s %s %d %s", item.id, item.name, item.category, &item.quantity, item.location) == 5) {
        printf("ID: %s | Name: %s | Quantity: %d | Location: %s | Category: %s\n",
               item.id, item.name, item.quantity, item.location, item.category);
    }

    fclose(file);
}

static void showBorrowLogs(void) {
    FILE *file;
    struct BorrowLog log;

    file = fopen("borrow.txt", "r");
    if (file == NULL) {
        printf("Could not open borrow log file.\n");
        return;
    }

    printf("\nBorrowing Records:\n");
    while (fscanf(file, "%s %s %s %d", log.studentId, log.equipmentId, log.borrowDate, &log.returned) == 4) {
        printf("Student ID: %s | Equipment ID: %s | Date: %s | Returned: %s\n",
               log.studentId, log.equipmentId, log.borrowDate,
               log.returned == 1 ? "Yes" : "No");
    }

    fclose(file);
}

static void processRequests(void) {
    struct Request reqs[100];
    int count = 0;

    FILE *file = fopen("requests.txt", "r");
    if (!file) {
        printf("Could not open requests.txt\n");
        return;
    }

    while (fscanf(file, "%s %s %s", reqs[count].studentId, reqs[count].equipmentId, reqs[count].status) == 3) {
        count++;
    }
    fclose(file);

    file = fopen("requests.txt", "w");
    if (!file) {
        printf("Could not open requests.txt\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(reqs[i].status, "pending") == 0) {
            printf("Request from %s for equipment %s\n", reqs[i].studentId, reqs[i].equipmentId);
            printf("Approve (A) or Reject (R)? ");
            char input;
            scanf(" %c", &input);
            if (input == 'A' || input == 'a')
                strcpy(reqs[i].status, "approved");
            else
                strcpy(reqs[i].status, "rejected");
        }
        fprintf(file, "%s %s %s\n", reqs[i].studentId, reqs[i].equipmentId, reqs[i].status);
    }

    fclose(file);
    printf("\nAll pending requests processed.\n");
}

static void showReport(void) {
    FILE *file;
    struct Request req;
    int approved = 0, rejected = 0, pending = 0;

    file = fopen("requests.txt", "r");
    if (!file) {
        printf("Could not open requests.txt\n");
        return;
    }

    while (fscanf(file, "%s %s %s", req.studentId, req.equipmentId, req.status) == 3) {
        if (strcmp(req.status, "approved") == 0)
            approved++;
        else if (strcmp(req.status, "rejected") == 0)
            rejected++;
        else if (strcmp(req.status, "pending") == 0)
            pending++;
    }

    fclose(file);

    printf("\nDepartment Report:\n");
    printf("Approved Requests: %d\n", approved);
    printf("Rejected Requests: %d\n", rejected);
    printf("Pending Requests: %d\n", pending);
}

void showDeptMenu(void) {
    int option;
    do {
        printf("\n=== Department Head Panel ===\n");
        printf("1. Show Equipment List\n");
        printf("2. Show Borrow Logs\n");
        printf("3. Process Student Requests\n");
        printf("4. Show Report\n");
        printf("5. Logout\n");
        printf("Choose option: ");
        scanf("%d", &option);

        switch (option) {
            case 1: displayEquipments(); break;
            case 2: showBorrowLogs(); break;
            case 3: processRequests(); break;
            case 4: showReport(); break;
            case 5: printf("Logging out...\n"); break;
            default: printf("Invalid option. Try again.\n");
        }

    } while (option != 5);
}

