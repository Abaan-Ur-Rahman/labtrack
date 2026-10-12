#include <stdio.h>
#include <string.h>

#include "labtrack/admin.h"
#include "labtrack/department.h"
#include "labtrack/input.h"
#include "labtrack/student.h"
#include "labtrack/technician.h"

static int login(char *user_role) {
    char inputuser[20], inputpass[20];
    char fileuser[20], filepass[20], filerole[20];
    int attempt = 0;

    while (attempt < 3) {
        printf("\nLOGIN\n");
        read_word("Enter your username: ", inputuser, sizeof inputuser);
        read_word("Enter the password: ", inputpass, sizeof inputpass);

        FILE *fp = fopen("login.txt", "r");
        if (fp == NULL) {
            printf("Error: \"login.txt\" was not found.\n");
            return 0;
        }

        while (fscanf(fp, "%19s %19s %19s", fileuser, filepass, filerole) == 3) {
            if (strcmp(inputuser, fileuser) == 0 && strcmp(inputpass, filepass) == 0) {
                strcpy(user_role, filerole);
                fclose(fp);

                if (strcmp(filerole, "admin") == 0) return 1;
                else if (strcmp(filerole, "technician") == 0) return 2;
                else if (strcmp(filerole, "student") == 0) return 3;
                else if (strcmp(filerole, "department") == 0) return 4;
                else {
                    printf("Unknown role: %s\n", filerole);
                    return 0;
                }
            }
        }

        fclose(fp);
        printf("Wrong username or password, try again.\n");
        attempt++;
    }

    printf("You failed login 3 times.\n");
    return 0;
}

static void main_menu(void) {
    int pick, login_result;
    char role[20];
    do {
        printf("\nMain Menu");
        printf("\n1. Admin");
        printf("\n2. Lab Technician");
        printf("\n3. Student");
        printf("\n4. Department Head");
        printf("\n5. Exit\n");
        pick = read_int("Enter your choice: ", 1, 5);

        switch (pick) {
            case 1:
                login_result = login(role);
                if (login_result == 1) admin_main();
                break;
            case 2:
                login_result = login(role);
                if (login_result == 2) lab_main();
                break;
            case 3:
                login_result = login(role);
                if (login_result == 3) studentMenu();
                break;
            case 4:
                login_result = login(role);
                if (login_result == 4) showDeptMenu();
                break;
            case 5:
                printf("Thank You!\n");
                break;
            default:
                printf("Please only pick a number from 1 to 5.\n");
        }
    } while (pick != 5);
}

int main(void) {
    main_menu();
    return 0;
}