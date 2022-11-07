node *currentAccount;
FILE *f;
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