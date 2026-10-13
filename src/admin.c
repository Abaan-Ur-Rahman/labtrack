#include "labtrack/admin.h"
#include "labtrack/input.h"
#include "labtrack/models.h"

#include <stdio.h>
#include <string.h>

static int last_id(void) {
    FILE *fp = fopen("equipment.txt", "r");
    Equipment e;
    int last = 0;

    if (fp != NULL) {
        while (fscanf(fp, "%d %s %s %d %s", &e.id, e.name, e.category, &e.quantity, e.location) != EOF) {
            last = e.id;
        }
        fclose(fp);
    }

    return last;
}

static void add_equipment(void) {
    FILE *fp = fopen("equipment.txt", "a");
    Equipment e;

    if (fp == NULL) {
        printf("File not found.\n");
        return;
    }

    e.id = last_id() + 1;

    read_word("Enter the equipment's name (Use _ instead of space): ", e.name, sizeof e.name);
    read_word("Enter the category (Use _ instead of space): ", e.category, sizeof e.category);
    e.quantity = read_int("Enter the quantity: ", 0, MAX_QUANTITY);
    read_word("Enter the location (Use _ instead of space): ", e.location, sizeof e.location);

    fprintf(fp, "%d %s %s %d %s\n", e.id, e.name, e.category, e.quantity, e.location);
    fclose(fp);

    printf("Equipment added successfully. ID = %d\n", e.id);
}

static void view_equipment(void) {
    FILE *fp = fopen("equipment.txt", "r");
    Equipment e;
    int found = 0;

    if (fp == NULL) {
        printf("Error \"equipment.txt\" was not found.\n");
        return;
    }

    printf("\nEquipment list:\n");
    while (fscanf(fp, "%d %s %s %d %s", &e.id, e.name, e.category, &e.quantity, e.location) != EOF){
        printf("Equipment ID: %d\n", e.id);
        printf("Name: %s\n", e.name);
        printf("Category: %s\n", e.category);
        printf("Quantity: %d\n", e.quantity);
        printf("Location: %s\n", e.location);
        printf("\n");
        found = 1;
    }

    fclose(fp);
    if (!found) {
        printf("No equipment found.\n");
    }
}

static void remove_equipment(void) {
    int id, found = 0;
    Equipment e;

    id = read_int("Enter equipment ID to remove the equipment: ", 1, MAX_ID);

    FILE *fp = fopen("equipment.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) {
        printf("Error file not found.\n");
        return;
    }

    while (fscanf(fp, "%d %s %s %d %s", &e.id, e.name, e.category, &e.quantity, e.location) != EOF) {
        if (e.id != id) {
            fprintf(temp, "%d %s %s %d %s\n", e.id, e.name, e.category, e.quantity, e.location);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("equipment.txt");
    rename("temp.txt", "equipment.txt");

    if (found) {
        printf("Equipment removed successfully.\n");
    } else {
        printf("Equipment ID not found.\n");
    }
}

static void update_equipment(void) {
    int id, found = 0;
    Equipment e;

    id = read_int("Enter equipment ID to update the equipment: ", 1, MAX_ID);

    FILE *fp = fopen("equipment.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) {
        printf("Error file not found.\n");
        return;
    }

    while (fscanf(fp, "%d %s %s %d %s", &e.id, e.name, e.category, &e.quantity, e.location) != EOF) {
        if (e.id == id) {
            read_word("Enter new name (Use _ instead of space): ", e.name, sizeof e.name);
            read_word("Enter new category (Use _ instead of space): ", e.category, sizeof e.category);
            e.quantity = read_int("Enter new quantity: ", 0, MAX_QUANTITY);
            read_word("Enter new location (Use _ instead of space): ", e.location, sizeof e.location);
            found = 1;
        }
        fprintf(temp, "%d %s %s %d %s\n", e.id, e.name, e.category, e.quantity, e.location);
    }

    fclose(fp);
    fclose(temp);
    remove("equipment.txt");
    rename("temp.txt", "equipment.txt");

    if (found) {
        printf("Equipment updated successfully.\n");
    } else {
        printf("Equipment ID not found.\n");
    }
}

static void admin_menu(void) {
    int choice;

    do {
        printf("\nADMIN MENU");
        printf("\n1. Add Equipment");
        printf("\n2. View Equipment");
        printf("\n3. Remove Equipment");
        printf("\n4. Update Equipment");
        printf("\n5. Logout\n");
        choice = read_int("Enter your choice: ", 1, 5);

        switch (choice) {
            case 1: add_equipment(); break;
            case 2: view_equipment(); break;
            case 3: remove_equipment(); break;
            case 4: update_equipment(); break;
            case 5: printf("Thank You!\n"); break;
            default: printf("Please only pick a number from 1 to 5.\n");
        }
    } while (choice != 5);
}

void admin_main(void) {
    admin_menu();
}