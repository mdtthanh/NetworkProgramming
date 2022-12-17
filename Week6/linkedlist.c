#include "linkedlist.h"

void addElement(node **root, account user)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->user = user;
    newNode->next = *root;
    *root = newNode;
}

void deleteElement(node **root, char *userName)
{
    node *temp1, *temp2;
    if (strcmp((*root)->user.userName, userName) == 0)
    {
        temp1 = *root;
        *root = (*root)->next;
        free(temp1);
    }
    else
    {
        temp1 = *root;
        temp2 = temp1->next;
        while (temp2 != NULL)
        {
            {
                if (strcmp(temp2->user.userName, userName) == 0)
                {
                    temp1->next = temp2->next;
                    free(temp2);
                }
                temp1 = temp1->next;
                temp2 = temp2->next;
            }
        }
    }
}

void showList(node *root)
{
    while (root != NULL)
    {
        {
            printf("%s %s %d\n", root->user.userName, root->user.password, root->user.status);
            root = root->next;
        }
    }
}

void freeList(node* head){
    node* tmp;
    while (head != NULL)
    {       
        tmp = head;
        head = head->next;
        free(tmp);
    }
    
}
