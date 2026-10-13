#include "labtrack/technician.h"
#include "labtrack/input.h"
#include "labtrack/models.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int student_id;
    int equipment_id;
    char borrow_date[11];
    int is_returned;
} BorrowRecord;

static Equipment inventory[50];
static BorrowRecord borrowRecords[100];
static int itemCount = 0;
static int borrowCount = 0;

static void loadEquipment(void) {
    FILE *file = fopen("equipment.txt", "r");
    if (!file) {
        printf("No inventory file found. Starting fresh.\n");
        return;
    }

    itemCount = 0;
    while (fscanf(file, "%d %49s %49s %d %49s",
                  &inventory[itemCount].id,
                  inventory[itemCount].name,
                  inventory[itemCount].category,
                  &inventory[itemCount].quantity,
                  inventory[itemCount].location) == 5) {
        itemCount++;
        if (itemCount >= 50) break;
    }
    fclose(file);
}

static void saveEquipment(void) {
    FILE *file = fopen("equipment.txt", "w");
    if (!file) {
        printf("Error saving equipment file!\n");
        return;
    }

    for (int i = 0; i < itemCount; i++) {
        fprintf(file, "%d %s %s %d %s\n",
                inventory[i].id,
                inventory[i].name,
                inventory[i].category,
                inventory[i].quantity,
                inventory[i].location);
    }
    fclose(file);
}

static void loadBorrowRecords(void) {
    FILE *file = fopen("borrow_records.txt", "r");
    if (!file) return;

    borrowCount = 0;
    while (fscanf(file, "%d %d %10s %d",
                  &borrowRecords[borrowCount].student_id,
                  &borrowRecords[borrowCount].equipment_id,
                  borrowRecords[borrowCount].borrow_date,
                  &borrowRecords[borrowCount].is_returned) == 4) {
        borrowCount++;
        if (borrowCount >= 100) break;
    }
    fclose(file);
}

static void saveBorrowRecords(void) {
    FILE *file = fopen("borrow_records.txt", "w");
    if (!file) {
        printf("Error saving borrow records!\n");
        return;
    }

    for (int i = 0; i < borrowCount; i++) {
        fprintf(file, "%d %d %s %d\n",
                borrowRecords[i].student_id,
                borrowRecords[i].equipment_id,
                borrowRecords[i].borrow_date,
                borrowRecords[i].is_returned);
    }
    fclose(file);
}

static void displayInventory(void) {
    printf("\n=== Current Inventory ===\n");
    printf("ID\tName\t\tCategory\tQuantity\tLocation\n");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < itemCount; i++) {
        printf("%d\t%-12s\t%-12s\t%d\t\t%-12s\n",
               inventory[i].id,
               inventory[i].name,
               inventory[i].category,
               inventory[i].quantity,
               inventory[i].location);
    }
}

static void issueEquipment(void) {
    int equipId, studentId;
    char date[11];

    printf("\n=== Issue Equipment ===\n");
    equipId = read_int("Enter Equipment ID: ", 1, MAX_ID);
    studentId = read_int("Enter Student ID: ", 1, MAX_ID);
    read_word("Enter Date (YYYY-MM-DD): ", date, sizeof date);

    for (int i = 0; i < itemCount; i++) {
        if (inventory[i].id == equipId) {
            if (inventory[i].quantity > 0) {
                inventory[i].quantity--;
                borrowRecords[borrowCount].student_id = studentId;
                borrowRecords[borrowCount].equipment_id = equipId;
                strcpy(borrowRecords[borrowCount].borrow_date, date);
                borrowRecords[borrowCount].is_returned = 0;
                borrowCount++;

                saveEquipment();
                saveBorrowRecords();
                printf("Success: %s issued to Student %d\n", inventory[i].name, studentId);
                return;
            } else {
                printf("Error: %s is out of stock!\n", inventory[i].name);
                return;
            }
        }
    }
    printf("Error: Equipment ID %d not found.\n", equipId);
}

static void returnEquipment(void) {
    int equipId, studentId, found = 0;

    printf("\n=== Return Equipment ===\n");
    equipId = read_int("Enter Equipment ID: ", 1, MAX_ID);
    studentId = read_int("Enter Student ID: ", 1, MAX_ID);

    for (int i = 0; i < borrowCount; i++) {
        if (borrowRecords[i].equipment_id == equipId &&
            borrowRecords[i].student_id == studentId &&
            !borrowRecords[i].is_returned) {

            borrowRecords[i].is_returned = 1;
            for (int j = 0; j < itemCount; j++) {
                if (inventory[j].id == equipId) {
                    inventory[j].quantity++;
                    break;
                }
            }
            found = 1;
            break;
        }
    }

    if (found) {
        saveEquipment();
        saveBorrowRecords();
        printf("Success: Equipment returned.\n");
    } else {
        printf("Error: No matching borrow record found.\n");
    }
}

static void addEquipment(void) {
    if (itemCount >= 50) {
        printf("Error: Inventory full!\n");
        return;
    }

    printf("\n=== Add New Equipment ===\n");
    Equipment *e = &inventory[itemCount];
    e->id = read_int("Enter Equipment ID: ", 1, MAX_ID);
    read_word("Enter Equipment Name: ", e->name, sizeof e->name);
    read_word("Enter Equipment Category: ", e->category, sizeof e->category);
    e->quantity = read_int("Enter Initial Quantity: ", 0, MAX_QUANTITY);
    read_word("Enter Equipment Location: ", e->location, sizeof e->location);

    itemCount++;
    saveEquipment();
    printf("Success: Equipment added!\n");
}

static void searchEquipment(void) {
    int choice, id;
    char query[50];

    printf("\n=== Search Equipment ===\n");
    printf("1. By ID\n2. By Name\n3. By Location\n4. By Category\n");
    choice = read_int("Enter choice (1-4): ", 1, 4);

    if (choice == 1) id = read_int("Enter search term: ", 1, MAX_ID);
    else read_word("Enter search term: ", query, sizeof query);

    printf("\n=== Search Results ===\n");
    printf("ID\tName\t\tCategory\tQuantity\tLocation\n");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < itemCount; i++) {
        int match = 0;
        switch (choice) {
            case 1: if (inventory[i].id == id) match = 1; break;
            case 2: if (strstr(inventory[i].name, query)) match = 1; break;
            case 3: if (strstr(inventory[i].location, query)) match = 1; break;
            case 4: if (strstr(inventory[i].category, query)) match = 1; break;
        }
        if (match) {
            printf("%d\t%-12s\t%-12s\t%d\t\t%-12s\n",
                   inventory[i].id,
                   inventory[i].name,
                   inventory[i].category,
                   inventory[i].quantity,
                   inventory[i].location);
        }
    }
}

static void generateUsageReport(void) {
    printf("\n=== Equipment Usage Report ===\n");
    printf("Student ID\tEquipment ID\tBorrow Date\tStatus\n");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < borrowCount; i++) {
        printf("%d\t\t%d\t\t%s\t\t%s\n",
               borrowRecords[i].student_id,
               borrowRecords[i].equipment_id,
               borrowRecords[i].borrow_date,
               borrowRecords[i].is_returned ? "Returned" : "Borrowed");
    }
}

static void showMenu(void) {
    printf("\n=== Lab Equipment Management System ===\n");
    printf("1. Display Inventory\n");
    printf("2. Issue Equipment\n");
    printf("3. Return Equipment\n");
    printf("4. Add New Equipment\n");
    printf("5. Search Equipment\n");
    printf("6. Generate Usage Report\n");
    printf("7. Exit\n");
}

void lab_main(void) {
    loadEquipment();
    loadBorrowRecords();

    int choice;
    do {
        showMenu();
        choice = read_int("Enter your choice: ", 1, 7);

        switch (choice) {
            case 1: displayInventory(); break;
            case 2: issueEquipment(); break;
            case 3: returnEquipment(); break;
            case 4: addEquipment(); break;
            case 5: searchEquipment(); break;
            case 6: generateUsageReport(); break;
            case 7: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 7);
}