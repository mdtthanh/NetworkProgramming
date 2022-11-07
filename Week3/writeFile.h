node *currentAccount;
FILE *f;
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
