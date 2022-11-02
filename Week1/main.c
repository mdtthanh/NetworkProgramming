#include <stdio.h>
#include <string.h>
#include "linkedlist.h"

node *currentAccount;
FILE *f;
void ReadFile(node **root);
void Register(node **root);
void Activate(node *root);
void Signin(node *root);
void Search(node *root);
void WriteFile(node *root);
void ChangePassword(node *root);
void SignOut(node *root);

int main()
{
    node *root = NULL;
    int option;
    ReadFile(&root);
    do
    {
        printf("\n\nUSER MANAGEMENT PROGRAM \n");
        printf("---------------------------------------------\n");
        printf("1. Register\n");
        printf("2. Activate\n");
        printf("3. Sign in\n");
        printf("4. Search\n");
        printf("5. Change password\n");
        printf("6. Signout\n");
        printf("7. Exit\n");
        printf("Your choice (1-7, other to quit): ");
        scanf("%d", &option);
        if (option == 1)
        {
            Register(&root);
        }
        else if (option == 2)
        {
            Activate(root);
        }
        else if (option == 3)
        {
            Signin(root);
        }
        else if (option == 4)
        {
            Search(root);
        }
        else if (option == 5)
        {
            ChangePassword(root);
        }
        else if (option == 6)
        {
            SignOut(root);
        }
        else if (option == 7)
        {
            break;
        }
    } while (option > 0 && option < 7);
    freeList(root);
    return 0;
}

void Register(node **root)
{
    node *value = *root;
    char name[50];
    char password[50];
    int checkAccount = 0;
    printf("------------Register------------\n");
    printf("Please Username: ");
    scanf("%s", name);
    while (value != NULL)
    {
        if (strcmp(value->user.userName, name) == 0)
        {
            checkAccount = 1;
        }
        value = value->next;
    }
    if (checkAccount == 1)
    {
        printf("Account exsisted\n");
    }
    else
    {
        printf("Password: ");
        scanf("%s", password);
        account newElement;
        strcpy(newElement.userName, name);
        strcpy(newElement.password, password);
        newElement.status = 2;
        addElement(root, newElement);
        WriteFile(*root);
        printf("Successful registration. Activation required.\n");
    }
}

void Activate(node *root)
{
    node *value = root;
    char name[50];
    char password[50];
    int checkAccount;
    printf("------------Register------------\n");
    printf("Username: ");
    scanf("%s", name);
    printf("Password: ");
    scanf("%s", password);
    int code;
    int check = 0;
    while (value != NULL)
    {
        if (strcmp(value->user.userName, name) == 0 && strcmp(value->user.password, password) == 0)
        {
            checkAccount = 1;
            break;
        }
        value = value->next;
    }
    if (checkAccount != 1)
    {
        printf("Username or password mismatch!\n");
    }
    else if (value->user.status == 0)
    {
        printf("Account was blocked!\n");
    }
    else if (value->user.status == 1)
    {
        printf("Account is activating!\n");
    }
    else if (value->user.status == 2)
    {
        while (check <= 4)
        {
            printf("Nhap Code: ");
            scanf("%d", &code);
            if (code != 20194675)
            {
                printf("Account is not activated\n");
                check++;
            }
            else
            {
                break;
            }
        }
        if (check > 4)
        {
            printf("Activation code is incorrect. Account is blocked.\n");
            value->user.status = 0;
            WriteFile(root);
        }
        else
        {
            value->user.status = 1;
            WriteFile(root);
            printf("Account is activated!\n");
        }
    }
}

void Signin(node *root)
{
    char name[50], password[50];
    int checkValue1 = 0;
    int checkValue2 = 0;
    node *value = root;
    printf("-------------Login------------\n");
    printf("Username: ");
    scanf("%s", name);
    while (value != NULL)
    {
        if (strcmp(value->user.userName, name) == 0 && value->user.status == 1)
        {
            checkValue1 = 1;
            break;
        }
        value = value->next;
    }
    if (checkValue1 != 1)
        printf("Cannot find account!\n");
    else if (value->user.status == 0)
    {
        printf("Account is blocked!\n");
        currentAccount = NULL;
    }
    else
    {
        do
        {
            printf("\nPassword: ");
            scanf("%s", password);
            if (strcmp(password, value->user.password) != 0)
            {
                if (checkValue2 == 2)
                {
                    value->user.status = 0;
                    WriteFile(root);
                    printf("Password is incorrect. Account is blocked\n");
                    break;
                }
                ++checkValue2;
                printf("Password incorrect!\n");
            }
            else
            {
                printf("Hello %s\n", name);
                currentAccount = value;
                break;
            }
        } while (checkValue2 < 3);
    }
}

void Search(node *root)
{
    char name[50];
    node *value = root;
    int checkAccount;
    printf("Username: ");
    scanf("%s", name);
    while (value != NULL)
    {
        {
            if (strcmp(value->user.userName, name) == 0)
            {
                checkAccount = 1;
                break;
            }
            value = value->next;
        }
    }
    if (checkAccount != 1)
    {
        printf("Cannot find account\n");
    }
    else if (value->user.status == 1)
    {
        printf("Account is active.\n");
    }
    else if (value->user.status == 0)
    {
        printf("Account is blocked");
    }
    else if (value->user.status == 2)
    {
        printf("Account is not active.\n");
    }
}

void ChangePassword(node *root)
{
    char password[50];
    char newPassword[50];
    node *value = root;
    if (currentAccount != NULL)
    {
        printf("Password: ");
        scanf("%s", password);
        while (1)
        {
            if (strcmp(currentAccount->user.password, password) != 0)
            {
                printf("Current password is incorrect. Please try again\n");
                printf("Password: ");
                scanf("%s", password);
            }
            else
            {
                printf("NewPassword: ");
                scanf("%s", newPassword);
                while (value != NULL)
                {
                    if (strcmp(currentAccount->user.userName, value->user.userName) == 0)
                    {
                        strcpy(value->user.password, newPassword);
                        WriteFile(root);
                        printf("Password is changed.\n");
                    }
                    value = value->next;
                }
                break;
            }
        }
    }
    else
    {
        printf("Not signin!\n");
    }
}

void SignOut(node *root)
{
    char name[50];
    int check;
    node *value = root;
    printf("-------------Logout------------\n");
    printf("Username: ");
    scanf("%s", name);
    if (currentAccount)
    {
        if (strcmp(currentAccount->user.userName, name) == 0)
        {
            printf("Goodbye %s!\n", currentAccount->user.userName);
            currentAccount = NULL;
        }
        else
        {
            while (value != NULL)
            {
                if (strcmp(value->user.userName, name) == 0)
                {
                    check = 1;
                    break;
                }
                value = value->next;
            }
            if (check != 1)
            {
                printf("Cannot find account!\n");
            }
            else
            {
                printf("Account is not sign in!\n");
            }
        }
    }
    else
    {
        while (value != NULL)
        {
            if (strcmp(value->user.userName, name) == 0)
            {
                check = 1;
                break;
            }
            value = value->next;
        }
        if (check != 1)
        {
            printf("Cannot find account!\n");
        }
        else
        {
            printf("Account is not sign in!\n");
        }
    }
}

void ReadFile(node **root)
{
    account temp;
    f = fopen("account.txt", "r");
    if (f != NULL)
    {
        while (fscanf(f, "%s %s %d", temp.userName, temp.password, &temp.status) != EOF)
        {
            addElement(root, temp);
        }
    }
    fclose(f);
}

void WriteFile(node *root)
{
    f = fopen("account.txt", "wt");
    while (root != NULL)
    {
        fprintf(f, "%s %s %d\n", root->user.userName, root->user.password, root->user.status);
        root = root->next;
    }
    fclose(f);
}
