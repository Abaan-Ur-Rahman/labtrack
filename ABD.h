#ifndef ABD_H
#define ABD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int id;
    char name[50];
    char category[50];  // Added category
    int quantity;
    char location[50];
} Equipment;

typedef struct {
    int student_id;
    int equipment_id;
    char borrow_date[11];
    int is_returned; 
} BorrowRecord;

Equipment inventory[50];
BorrowRecord borrowRecords[100];
int itemCount = 0;
int borrowCount = 0;

void loadEquipment() {
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

void saveEquipment() {
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

void loadBorrowRecords() {
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

void saveBorrowRecords() {
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

void displayInventory() {
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

void issueEquipment() {
    int equipId, studentId;
    char date[11];

    printf("\n=== Issue Equipment ===\n");
    printf("Enter Equipment ID: ");
    scanf("%d", &equipId);
    printf("Enter Student ID: ");
    scanf("%d", &studentId);
    printf("Enter Date (YYYY-MM-DD): ");
    scanf("%10s", date);

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

void returnEquipment() {
    int equipId, studentId, found = 0;

    printf("\n=== Return Equipment ===\n");
    printf("Enter Equipment ID: ");
    scanf("%d", &equipId);
    printf("Enter Student ID: ");
    scanf("%d", &studentId);

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

void addEquipment() {
    if (itemCount >= 50) {
        printf("Error: Inventory full!\n");
        return;
    }

    printf("\n=== Add New Equipment ===\n");
    printf("Enter Equipment ID: ");
    scanf("%d", &inventory[itemCount].id);
    printf("Enter Equipment Name: ");
    scanf("%49s", inventory[itemCount].name);
    printf("Enter Equipment Category: ");
    scanf("%49s", inventory[itemCount].category);
    printf("Enter Initial Quantity: ");
    scanf("%d", &inventory[itemCount].quantity);
    printf("Enter Equipment Location: ");
    scanf("%49s", inventory[itemCount].location);

    itemCount++;
    saveEquipment();
    printf("Success: Equipment added!\n");
}

void searchEquipment() {
    int choice, id;
    char query[50];

    printf("\n=== Search Equipment ===\n");
    printf("1. By ID\n2. By Name\n3. By Location\n4. By Category\n");
    printf("Enter choice (1-4): ");
    scanf("%d", &choice);

    printf("Enter search term: ");
    if (choice == 1) scanf("%d", &id);
    else scanf("%49s", query);

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

void generateUsageReport() {
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

void showMenu() {
    printf("\n=== Lab Equipment Management System ===\n");
    printf("1. Display Inventory\n");
    printf("2. Issue Equipment\n");
    printf("3. Return Equipment\n");
    printf("4. Add New Equipment\n");
    printf("5. Search Equipment\n");
    printf("6. Generate Usage Report\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

void lab_main() {
    loadEquipment();
    loadBorrowRecords();

    int choice;
    do {
        showMenu();
        scanf("%d", &choice);

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

#endif