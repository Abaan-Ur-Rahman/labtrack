#ifndef ADMIN_H
#define ADMIN_H

#include <stdio.h>
#include <string.h>

struct equipment {
    int id;
    char name[50];
    char category[50];
    int quantity;
    char location[50];
};

int string_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

void clearInput() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int last_id() {
    FILE *fp = fopen("equipment.txt", "r");
    struct equipment e;
    int last = 0;

    if (fp != NULL) {
        while (fscanf(fp, "%d %s %s %d %s", &e.id, e.name, e.category, &e.quantity, e.location) != EOF) {
            last = e.id;
        }
        fclose(fp);
    }

    return last;
}

void add_equipment() {
    FILE *fp = fopen("equipment.txt", "a");
    struct equipment e;

    if (fp == NULL) {
        printf("File not found.\n");
        return;
    }

    e.id = last_id() + 1;

    clearInput();
    printf("Enter the equipment's name (Use _ instead of space): ");
    scanf("%s", e.name);
    printf("Enter the category (Use _ instead of space): ");
    scanf("%s", e.category);
    printf("Enter the quantity: ");
    scanf("%d", &e.quantity);
    clearInput();
    printf("Enter the location (Use _ instead of space): ");
    scanf("%s", e.location);

    fprintf(fp, "%d %s %s %d %s\n", e.id, e.name, e.category, e.quantity, e.location);
    fclose(fp);

    printf("Equipment added successfully. ID = %d\n", e.id);
}

void view_equipment() {
    FILE *fp = fopen("equipment.txt", "r");
    struct equipment e;
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

void remove_equipment() {
    int id, found = 0;
    struct equipment e;

    printf("Enter equipment ID to remove the equipment: ");
    scanf("%d", &id);

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

void update_equipment() {
    int id, found = 0;
    struct equipment e;

    printf("Enter equipment ID to update the equipment: ");
    scanf("%d", &id);

    FILE *fp = fopen("equipment.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) {
        printf("Error file not found.\n");
        return;
    }

    while (fscanf(fp, "%d %s %s %d %s", &e.id, e.name, e.category, &e.quantity, e.location) != EOF) {
        if (e.id == id) {
            clearInput();
            printf("Enter new name (Use _ instead of space): ");
            scanf("%s", e.name);
            printf("Enter new category (Use _ instead of space): ");
            scanf("%s", e.category);
            printf("Enter new quantity: ");
            scanf("%d", &e.quantity);
            clearInput();
            printf("Enter new location (Use _ instead of space): ");
            scanf("%s", e.location);
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

void admin_menu() {
    int choice;

    do {
        printf("\nADMIN MENU");
        printf("\n1. Add Equipment");
        printf("\n2. View Equipment");
        printf("\n3. Remove Equipment");
        printf("\n4. Update Equipment");
        printf("\n5. Logout");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

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

void admin_main() {
    admin_menu();
}

#endif