#include "admin.c"
#include "technician.c"
#include "student.c"
#include "department.c"

int login(char *user_role) {
    char inputuser[20], inputpass[20];
    char fileuser[20], filepass[20], filerole[20];
    int attempt = 0;

    while (attempt < 3) {
        printf("\nLOGIN\n");
        printf("Enter your username: ");
        scanf("%s", inputuser);
        printf("Enter the password: ");
        scanf("%s", inputpass);

        FILE *fp = fopen("login.txt", "r");
        if (fp == NULL) {
            printf("Error: \"login.txt\" was not found.\n");
            return 0;
        }

        int found = 0;
        while (fscanf(fp, "%s %s %s", fileuser, filepass, filerole) == 3) {
            if (string_equal(inputuser, fileuser) && string_equal(inputpass, filepass)) {
                strcpy(user_role, filerole);
                fclose(fp);

                if (string_equal(filerole, "admin")) return 1;
                else if (string_equal(filerole, "technician")) return 2;
                else if (string_equal(filerole, "student")) return 3;
                else if (string_equal(filerole, "department")) return 4;
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

void main_menu() {
    int pick, login_result;
    char role[20];
    do {
        printf("\nMain Menu");
        printf("\n1. Admin");
        printf("\n2. Lab Technician");
        printf("\n3. Student");
        printf("\n4. Department Head");
        printf("\n5. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &pick);

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

int main(){
    main_menu();
    return 0;
}