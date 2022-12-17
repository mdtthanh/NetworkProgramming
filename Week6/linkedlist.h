#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct account 
{
    char userName[50];
    char password[50];
    int status;
} account;

typedef struct node {
    account user;
    int login;
    struct node *next;
} node;

void addElement(node **root, account user);
void deleteElement(node **root, char *username);
void showList(node *root);
void freeList(node *head);