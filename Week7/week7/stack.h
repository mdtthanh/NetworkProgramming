#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ACTIVE 1
#define BLOCKED 0
#define idle 2
#define NOT_EXIST 3
#define WRONG_PASSWORD 5
#define ACCTIVE_ACCOUNT_CODE 20184040
#define LIMIT_INPUT_WRONG 4
typedef struct DT
{
	char userName[100];
	char password[100];
	int status;
	int totalTimeWrongCode;
	int totalTimeWrongPassword;
} DT;
typedef struct NODE
{
	DT x;
	struct NODE *next;
} NODE;
typedef struct LIST
{
	NODE *Head;
} LIST;
int isEmpty(LIST *l);
NODE *CreatNODE(DT x);
void Push(LIST *l, DT x);
DT Pop(LIST *l);
void Free(LIST *l);
LIST *SeverseLIST(LIST *l);
