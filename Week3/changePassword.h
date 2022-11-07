node *currentAccount;
FILE *f;



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

