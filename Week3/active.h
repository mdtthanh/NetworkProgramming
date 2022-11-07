node *currentAccount;
FILE *f;
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