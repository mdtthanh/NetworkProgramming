node *currentAccount;
FILE *f;
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
