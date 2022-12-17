#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "stack.h"
#include <pthread.h> //for threading , link with lpthread

#define MAXLINE 1024
#define SUCCESSFULL 1
#define ERROR 0
#define SA struct sockaddr

NODE *userLoged = NULL;
int checkingStatus = 0;
char passwordDigit[MAXLINE];
char passwordAlpha[MAXLINE];

struct sockaddr_in servaddr, cliaddr;
int sockfd;
socklen_t len = sizeof(cliaddr);
int firstCheckInput = 0;
int sockfd, connect_status;
struct sockaddr_in servaddr, cli;
char username[MAXLINE];
char password[MAXLINE];
char *insertPassword = "Insert Password";

pid_t childpid;

void DNODE(DT x)
{
	printf("%-30s%-20s%-15d%-10d\n", x.userName, x.password, x.status, x.totalTimeWrongCode);
}

void DLIST(LIST *l)
{
	NODE *p = l->Head;
	printf("%-35s%-20s%-15s%-10s\n", "Tên đăng nhập", "Mật khẩu", "Trạng thái", "Số lần đã nhập sai");
	while (p != NULL)
	{
		DNODE(p->x);
		p = p->next;
	}
}
void Empty(LIST *l)
{
	l->Head = NULL;
}

void writeFileAfterUpdate(LIST *l)
{
	FILE *fptr = fopen("nguoidung.txt", "w");
	NODE *p = l->Head;
	while (p != NULL)
	{

		fwrite(p->x.userName, 1, strlen(p->x.userName), fptr);
		fwrite("\t", 1, 1, fptr);

		fwrite(p->x.password, 1, strlen(p->x.password), fptr);
		fwrite("\t", 1, 1, fptr);

		char convertTest[5];
		sprintf(convertTest, "%d", p->x.status);
		fwrite(convertTest, 1, strlen(convertTest), fptr);
		if (p->next != NULL)
		{
			fwrite("\n", 1, 1, fptr);
		}
		p = p->next;
	}
	fclose(fptr);
}

void writeFileHaveErrorCode(LIST *l)
{
	FILE *fptr = fopen("nguoidung2.txt", "w");
	NODE *p = l->Head;
	while (p != NULL)
	{

		fwrite(p->x.userName, 1, strlen(p->x.userName), fptr);
		fwrite("\t", 1, 1, fptr);

		fwrite(p->x.password, 1, strlen(p->x.password), fptr);
		fwrite("\t", 1, 1, fptr);

		char convertTest[5];
		sprintf(convertTest, "%d", p->x.status);
		fwrite(convertTest, 1, strlen(convertTest), fptr);

		fwrite("\t", 1, 1, fptr);

		sprintf(convertTest, "%d", p->x.totalTimeWrongPassword);
		fwrite(convertTest, 1, strlen(convertTest), fptr);
		if (p->next != NULL)
		{
			fwrite("\n", 1, 1, fptr);
		}

		p = p->next;
	}
	fclose(fptr);
}

NODE *findAnAccount(LIST *l, char userName[100])
{
	NODE *p = l->Head;
	while (p != NULL)
	{
		if (strcmp(userName, p->x.userName) == 0)
		{
			return p;
			break;
		}
		p = p->next;
	}
	return NULL;
}

char *convertValue(long status)
{
	switch (status)
	{
	case ACTIVE:
		return "1";
		break;

	case NOT_EXIST:
		return "3";
		break;

	case BLOCKED:
		return "0";
		break;

	case WRONG_PASSWORD:
		return "5";
		break;

	default:
		return "0";
		break;
	}
}

int encodePassword(char *password)
{

	int index1 = 0, index2 = 0;
	memset(passwordAlpha, 0, sizeof(passwordAlpha));
	memset(passwordDigit, 0, sizeof(passwordDigit));

	for (int i = 0; i < strlen(password); i++)
	{
		// Check digit case
		if (isdigit(password[i]))
			passwordDigit[index1++] = password[i];
		else if ((password[i] >= 'a' && password[i] <= 'z') || (password[i] >= 'A' && password[i] <= 'Z'))
			passwordAlpha[index2++] = password[i];
		else
		{
			// Wrong case
			return 0;
		}
	}
	passwordDigit[index1] = '\0';
	passwordAlpha[index2] = '\0';

	return 1;
}

int changePassword(char *newPassword, LIST *l)
{

	if (!encodePassword(newPassword))
	{
		return ERROR;
	}
	strcpy(userLoged->x.password, newPassword);
	writeFileAfterUpdate(l);
	writeFileHaveErrorCode(l);
	return SUCCESSFULL;
}

void actionAfterChangePassword(int connect_status, char *newPassword, LIST *l)
{
	int changePasswordStatus = changePassword(newPassword, l);

	// In case have special character => out => send error to client
	if (changePasswordStatus == ERROR)
	{
		printf("ERROR\n");
		write(connect_status, (const char *)"Fail", strlen("Fail"));
	}
	else
	{
		write(connect_status, (const char *)"successful", strlen("successful"));

		char buff[MAXLINE];
		memset(buff, '\0', sizeof(buff));

		read(connect_status, (char *)buff, MAXLINE);
		bzero(buff, MAXLINE);

		// Send message

		if (strlen(passwordAlpha) == 0)
		{
			strcpy(passwordAlpha, " ");
		}
		write(connect_status, (const char *)passwordAlpha, strlen(passwordAlpha));
		read(connect_status, (char *)buff, MAXLINE);

		if (strlen(buff) != 0)
		{

			if (strlen(passwordDigit) == 0)
			{
				strcpy(passwordDigit, " ");
			}
			write(connect_status, (const char *)passwordDigit, strlen(passwordDigit));
		}
		bzero(buff, MAXLINE);
		bzero(passwordAlpha, MAXLINE);
		bzero(passwordDigit, MAXLINE);
	}
}

// Driver code

void readFile(FILE *f, LIST *l)
{
	int lineCheck = 1;
	while (!feof(f))
	{
		DT x;
		x.totalTimeWrongCode = 0;
		x.totalTimeWrongPassword = 0;

		fscanf(f, "%s %s %d", x.userName, x.password, &x.status);
		NODE *checkAccount = findAnAccount(l, x.userName);
		if (checkAccount != NULL)
		{
			printf("\n======================================\n");
			printf("Username input in line %d have existed. Check you file!\n", lineCheck);
			printf("======================================\n");
			exit(0);
		}
		Push(l, x);
		lineCheck++;
	}

	writeFileHaveErrorCode(l);
	// printf("\n\nYour file valid! Below is you data after insert:\n\n");
	// DLIST(l);
}

// void readFileWithErrorNumber(FILE *f, LIST *l)
// {
// 	int lineCheck = 1;
// 	while (!feof(f))
// 	{
// 		DT x;
// 		x.totalTimeWrongCode = 0;
// 		x.totalTimeWrongPassword = 0;

// 		fscanf(f, "%s %s %d %d", x.userName, x.password, &x.status, &x.totalTimeWrongPassword);
// 		NODE *checkAccount = findAnAccount(l, x.userName);
// 		if (checkAccount != NULL)
// 		{
// 			printf("\n======================================\n");
// 			printf("Username input in line %d have existed. Check you file!\n", lineCheck);
// 			printf("======================================\n");
// 			exit(0);
// 		}
// 		Push(l, x);
// 		lineCheck++;
// 	}
// 	// printf("\n\nYour file valid! Below is you data after insert:\n\n");
// 	// DLIST(l);
// }
int loginAccount(LIST *l, char *userName, char *password)
{

	FILE *fp = fopen("nguoidung2.txt", "r+");
	if (fp == NULL)
	{
		perror("Can not open this file. Check again your file name.");
		exit(0);
	}

	// readFileWithErrorNumber(fp, l);
	fclose(fp);

	NODE *user = findAnAccount(l, userName);
	if (user == NULL)
	{
		return NOT_EXIST;
	}

	if (user->x.status == BLOCKED)
	{
		return BLOCKED;
	}

	if (user->x.totalTimeWrongPassword == 3)
	{
		user->x.status = BLOCKED;
		writeFileAfterUpdate(l);
		writeFileHaveErrorCode(l);
		return BLOCKED;
	}

	if (strcmp(user->x.password, password) == 0)
	{
		if (user->x.status != ACTIVE)
		{
			return BLOCKED;
		}
		user->x.totalTimeWrongPassword = 0;
		writeFileHaveErrorCode(l);
		userLoged = user;
		return ACTIVE;
	}
	user->x.totalTimeWrongPassword++;
	writeFileHaveErrorCode(l);
	return WRONG_PASSWORD;
}
void *connection_handler(void *socket_desc);

LIST *l;

int main(int argc, char *argv[])
{
	int *new_sock;

	l = (LIST *)malloc(sizeof(LIST));
	Empty(l);

	if (argc < 2)
	{
		printf("Invalid argument. The model using is: ./server {{PORT}}\n");
		exit(0);
	}
	int port = atoi(argv[1]);

	printf("port using: %d\n", port);

	FILE *fp;

	fp = fopen("nguoidung.txt", "r+");
	if (fp == NULL)
	{
		perror("Can not open this file. Check again your file name.");
		exit(0);
	}
	readFile(fp, l);
	DLIST(l);
	fclose(fp);

	// int n = 0;

	// Creating socket file descriptor
	printf("Server started and listen on PORT: %d\n", port);
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	// Bind the socket with the server address
	if (bind(sockfd, (const struct sockaddr *)&servaddr,
			 sizeof(servaddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0)
	{
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	while (1)
	{
		// newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);
		connect_status = accept(sockfd, (SA *)&cli, &len);
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = connect_status;

		if (connect_status < 0)
		{
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));

		fp = fopen("nguoidung2.txt", "r+");
		if (fp == NULL)
		{
			perror("Can not open this file. Check again your file name.");
			exit(0);
		}
		// readFileWithErrorNumber(fp, l);
		fclose(fp);

		if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
	}

	return 0;
}

void *connection_handler(void *socket_desc)
{
	int sock = *(int *)socket_desc;
	int read_size;
	char *message, client_message[2000];
	bzero(client_message, 2000);

	while (1)
	{
		printf("Again\n");
		// Reset string receive
		memset(username, 0, sizeof(username));
		// Recv username
		read(sock, (char *)username, MAXLINE);
		if (strlen(username) == 0)
		{
			break;
		}
		printf("username: %s\n", username);

		if (strcmp(username, "") == 0)
		{
			printf("Turn off server...\n");
			exit(0);
		}

		write(sock, (const char *)insertPassword, strlen(insertPassword));

		memset(password, 0, sizeof(password));

		read(sock, (char *)password, MAXLINE);

		int i_valueAfterLogin = loginAccount(l, username, password);

		// Check account after submit username and password
		char *test2 = convertValue(i_valueAfterLogin);
		write(sock, (const char *)test2, strlen(test2));
		char *activeStatus = "1";
		if (strcmp(test2, activeStatus) == 0)
		{
			char passwordWantChange[MAXLINE];
			memset(passwordWantChange, 0, sizeof(passwordWantChange));
			// 1st time
			read(sock, (char *)passwordWantChange, MAXLINE);

			// if (strcmp(passwordWantChange, "") == 0)
			// {
			// 	exit(0);
			// }
			while (strcmp(passwordWantChange, "bye") != 0)
			{
				if (strcmp(passwordWantChange, "") == 0)
				{
					// exit(0);
					return;
				}

				actionAfterChangePassword(sock, passwordWantChange, l);
				// Reset string and listen for next action
				bzero(passwordWantChange, sizeof(passwordWantChange));
				read(sock, (char *)passwordWantChange, MAXLINE);
			}

			if (strcmp(passwordWantChange, "bye") == 0)
			{
				userLoged = NULL;
			}
		}
	}
}