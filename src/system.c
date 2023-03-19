#include "header.h"

const char *RECORDS = "./data/records.txt";

void saveChanges() {
    remove("./data/records.txt");
    rename("./data/temp.txt", "./data/records.txt");
}

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r) {
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User *u, struct Record *r) {
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r->id,
            u->id,
            u->name,
            r->accountNbr,
            r->deposit.month,
            r->deposit.day,
            r->deposit.year,
            r->country,
            r->phone,
            r->amount,
            r->accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u) {
    int option;
    if (notGood == 0) {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    } else {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1) {
        system("clear");
        mainMenu(u);
    } else {
        system("clear");
        exit(1);
    }
}

void success(struct User u) {
    int option;
    printf("\n✅ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1) {
        mainMenu(u);
    } else if (option == 0) {
        exit(1);
    } else {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void CheckDetailsAccount(struct User u) {
    int accountNum, date, d, m, y;
    char userName[100], accountType[100];
    struct Record r;
    float InterestRate = 1.0;
    int ok = 0;

Again:
    printf("Enter the account number: ");
    scanf("%d", &accountNum);

    system("clear");

    FILE *f = fopen(RECORDS, "r");

    if ((f = fopen(RECORDS, "r+")) == NULL) {
        printf("❌ Error opening file");
        exit(1);
    }
    while (getAccountFromFile(f, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && accountNum == r.accountNbr) {
            ok++;
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n", r.accountNbr, r.deposit.day, r.deposit.month, r.deposit.year, r.country, r.phone, r.amount, r.accountType);
            if (strcmp(r.accountType, "saving") == 0) {
                InterestRate = 0.07;
                strcpy(accountType, "saving");
                date = r.deposit.day;
            } else {
                if (strcmp(r.accountType, "current") == 0) {
                    InterestRate = 0.0;
                    strcpy(accountType, "current");
                }
                if (strcmp(r.accountType, "fixed01") == 0) {
                    InterestRate = 0.04;
                    strcpy(accountType, "fixed01");
                }
                if (strcmp(r.accountType, "fixed02") == 0) {
                    InterestRate = 0.05;
                    strcpy(accountType, "fixed02");
                }
                if (strcmp(r.accountType, "fixed03") == 0) {
                    InterestRate = 0.08;
                    strcpy(accountType, "fixed03");
                }
                d = r.deposit.day;
                m = r.deposit.month;
                y = r.deposit.year;
            }
        }
    }
    if (ok == 0) {
        printf("❌ Error Invalid operation\n\n");
        stayOrReturn(1, mainMenu, u);
        goto Again;
    }

    if (InterestRate == 0.0) {
        printf("\n\nYou will not get interests because the account is of type current");
    } else {
        if (strcmp(accountType, "saving") == 0) {
            printf("\n\nYou will get $%f as interest on day %d of every month\n", (r.amount * InterestRate) / 12, date);
        } else {
            if (strcmp(accountType, "fixed01") == 0) {
                printf("\n\nYou will get $%f as interest on %d/%d/%d", (r.amount * InterestRate), d, m, y + 1);
            }
            if (strcmp(accountType, "fixed02") == 0) {
                printf("\n\nYou will get $%f as interest on %d/%d/%d", (r.amount * InterestRate) * 2, d, m, y + 2);
            }
            if (strcmp(accountType, "fixed03") == 0) {
                printf("\n\nYou will get $%f as interest on %d/%d/%d", (r.amount * InterestRate) * 3, d, m, y + 3);
            }
        }
    }
    success(u);
}

void removeAccount(struct User u) {
    int AccNum;
    FILE *f;
    struct Record r;
    char userName[100];
    int ok = 0;

    if ((f = fopen(RECORDS, "r+")) == NULL) {
        printf("❌ Error opening file");
        exit(1);
    }

    system("clear");
    printf("\n\nEnter the account number to delete: ");
    scanf("%d", &AccNum);

    FILE *new;
    if ((new = fopen("./data/temp.txt", "w+")) == NULL) {
        printf("❌ Error opening file");
        exit(1);
    }

    while (getAccountFromFile(f, userName, &r)) {
        if ((strcmp(userName, u.name) == 0) && AccNum == r.accountNbr) {
            ok = 1;
            system("clear");
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n", r.accountNbr, r.deposit.day, r.deposit.month, r.deposit.year, r.country, r.phone, r.amount, r.accountType);
        } else {
            saveAccountToFile(new, &u, &r);
        }
    }
    if (ok == 0) {
        printf("Invalid operation\n\n");
        stayOrReturn(1, mainMenu, u);
    }

    saveChanges();

    fclose(f);
    fclose(new);

    success(u);
}

void createNewAccount(struct User u) {
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, userName, &cr)) {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr) {
            printf("❌Error This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    saveAccountToFile(pf, &u, &r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u) {
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0) {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void updateAccount(struct User u) {
    struct Record newRecord;
    char Country[100];

    FILE *f;
    struct Record r;
    char userName[50];

    if ((f = fopen(RECORDS, "r+")) == NULL) {
        printf("❌ Error opening file");
        exit(1);
    }

    system("clear");
    printf("\n\nThe account number to update: ");
    scanf("%d", &newRecord.accountNbr);
    int ok = 0;
    while (getAccountFromFile(f, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && newRecord.accountNbr == r.accountNbr) {
            ok = 1;
        }
    }
    if (ok == 0) {
        printf("\n❌ Invalid Account Number");
        stayOrReturn(1, mainMenu, u);
    }

    int field;
    printf("\nEnter the field to update?\n [1] - phone number\n [2] - country\n");
    scanf("%d", &field);

    system("clear");

    if (field == 1) {
        printf("Enter the new phone number: ");
        scanf("%d", &newRecord.phone);
    } else if (field == 2) {
        printf("Enter the new country: ");
        scanf("%s", newRecord.country);
    } else {
        printf("Invalid operation!\n");
        stayOrReturn(1, mainMenu, u);
    }
    FILE *new;
    if ((new = fopen("./data/temp.txt", "w")) == NULL) {
        printf("❌ Error opening file");
        exit(1);
    }

    struct Record r2;
    userName[0] = '\0';

    if ((f = fopen(RECORDS, "r+")) == NULL) {
        printf("❌ Error opening file");
        exit(1);
    }

    while (getAccountFromFile(f, userName, &r2)) {
        if (strcmp(userName, u.name) == 0 && newRecord.accountNbr == r2.accountNbr) {
            if (field == 1) {
                r2.phone = newRecord.phone;
            }
            if (field == 2) {
                strcpy(r2.country, newRecord.country);
            }
        }
        saveAccountToFile(new, &u, &r2);
    }

    saveChanges();

    fclose(f);
    fclose(new);
    success(u);
}
