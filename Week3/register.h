node *currentAccount;
FILE *f;
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
