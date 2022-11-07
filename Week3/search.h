node *currentAccount;
FILE *f;
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
