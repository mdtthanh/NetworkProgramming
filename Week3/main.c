#include <stdio.h>
#include <string.h>
#include "linkedlist.h"
#include "readFile.h"
#include "writeFile.h"
#include "register.h"
#include "active.h"
#include "search.h"
#include "changePassword.h"
#include "signOut.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "getIP.h"
#include "getHostname.h"
// #include "prossesIpAndHostname.h"
node *currentAccount;
FILE *f;
// void ReadFile(node **root);
// void Register(node **root);
// void Activate(node *root);
// void Signin(node *root);
// void Search(node *root);
// void WriteFile(node *root);
// void ChangePassword(node *root);
// void SignOut(node *root);

int main(int argc, char const *argv[])
{
    node *root = NULL;
    ReadFile(&root);
    char string[30];

    strcpy(string, argv[2]);

    if (argc != 3)
    {
        printf("Wrong parameter.\n");
    }

    if (strcmp(argv[1], "2") == 0)
    { /* string is a name */
        getIP(string);
    }
    else if (strcmp(argv[1], "1") == 0)
    {
        getHostname(string); /* string is an ip address */
    }

    else
    {
        printf("Wrong parameter.\n");
    }
    char option[10];

    int input;
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
        printf("7. Homepage with domain name\n");
        printf("8. Homepage with IP address\n");
        printf("9. Exit\n");
        printf("Your choice (1-9, other to quit): ");
        scanf("%[^\n]s", option);
        if (!(strlen(option) == 1 && (option[0] > 48 && option[0] < 57)))
        {
            // Exit program
            exit(EXIT_SUCCESS);
        }

        input = option[0] - 48;
        switch (input)
        {
        case 1:
            Register(&root);
            break;
        case 2:
            Activate(root);
            break;
        case 3:
            Signin(root);
            break;
        case 4:
            Search(root);
            break;
        case 5:
            ChangePassword(root);
            break;
        case 6:
            SignOut(root);
            break;
        case 7:
            getIP(root);
            break;
        case 8:
            getHostname(string);
            break;
        case 9:
            exit(1);
            break;
        default:
            break;
        }
    } while (1);
    freeList(root);
    return 0;
}
