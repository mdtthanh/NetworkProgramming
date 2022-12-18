#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFF_SIZE 1024
#define BACKLOG 2
#define MAX 256

enum
{
    user_register = 1,
    sign_in,
    search,
    sign_out
};

enum
{
    false,
    true
};

enum
{
    blocked,
    active
};

// input file
char filename[MAX] = "account.txt";

typedef struct Account
{
    char username[MAX];
    char password[MAX];
    int status;
    int login;
    int falseLogin;
    struct Account *next;
} account;

account *head = NULL;
account *current = NULL;

// print all linked list
void PrintList()
{
    account *p = head;
    printf("\n");
    while (p != NULL)
    {
        printf("Name: %-15s - Pass: %-10s - Status: %d - IsLogin: %d - NumOfFalseLogin: %d\n", p->username, p->password, p->status, p->login, p->falseLogin);
        p = p->next;
    }
}

void InsertFirst(account acc)
{
    account *link = (account *)malloc(sizeof(account));
    strcpy(link->username, acc.username);
    strcpy(link->password, acc.password);
    link->next = head;
    head = link;
}

void ReadData()
{
    account acc;
    int i = 0;
    FILE *f;

    if (!(f = fopen(filename, "r")))
    {
        printf("\n File not found!! \n\n");
    }
    else
    {
        while (!feof(f))
        {
            fscanf(f, "%s %s %d", acc.username, acc.password, &acc.status);
            acc.login = 0;
            acc.falseLogin = 0;
            InsertFirst(acc);
        }
    }
    fclose(f);
}

void ReWriteData()
{
    FILE *f;

    if (!(f = fopen(filename, "r")))
    {
        printf("\n File not found!! \n\n");
    }
    else
    {
        account *p;
        p = head;
        while (p != NULL)
        {
            fprintf(f, "%s %s %d\n", p->username, p->password, p->status);
            p = p->next;
        }
    }
    fclose(f);
}

// check if usename is in list
int isExisting(char *input)
{
    int isExist = false;
    account *ptr;
    ptr = head;
    while (ptr != NULL)
    {
        if (strcmp(input, ptr->username) == 0)
            isExist = true;
        ptr = ptr->next;
    }
    return isExist;
}

// sign in function
void UserSignIn(int connection_socket, char username[], char password[])
{
    int isDone = false;
    printf("--- Sign in ---\n");
    printf("Username: %s\n", username);
    printf("Password: %s\n", password);

    do
    {
        account *ptr;
        ptr = head;
        int isExist = false;
        while (ptr != NULL)
        {
            // check username exist or not
            if (strcmp(ptr->username, username) == 0)
            {
                // if username is found in list
                isExist = true;

                // check status
                if (ptr->status == 0)
                {
                    char *msg = "This account is blocked\n";
                    send(connection_socket, msg, strlen(msg), 0);
                    printf("Account \"%s\" is blocked.\n", ptr->username);
                }
                else
                {
                    // check password
                    if (strcmp(ptr->password, password) != 0)
                    {
                        char *msg = "Password is incorrect.\n";
                        ptr->falseLogin++;

                        // if after 3 time wrong password => status = blocked

                        if (ptr->falseLogin >= 3)
                        {
                            ptr->status = blocked;
                            isDone = true;
                            ptr->falseLogin = 0;

                            // rewrite data in file
                            ReWriteData();
                            char firstloglocation[BUFF_SIZE];
                            strcpy(firstloglocation, msg);
                            msg = strcat(firstloglocation, "Password is incorrect 3 times. Your account are blocked.\n");
                            send(connection_socket, msg, strlen(msg), 0);
                            printf("Password is incorrect 3 times. Account \"%s\" is blocked. \n Contract administractor for more information.\n", ptr->username);
                        }
                        else
                        {
                            send(connection_socket, msg, strlen(msg), 0);
                            printf("%s\n", msg);
                        }
                    }

                    else
                    {
                        isDone = true;
                        ptr->login = true;
                        ptr->falseLogin = 0;
                        char *msg = "Successful login! Hello ";
                        strcat(msg, ptr->username);
                        send(connection_socket, msg, strlen(msg), 0);
                        printf("Sign in Successful.\n");
                    }
                }
            }
            ptr = ptr->next;
        }
        if (isExist == false)
        {
            char *msg = "This account not exits!\n";
            send(connection_socket, msg, strlen(msg), 0);
            printf("Account \"%s\" not exits.\n", username);
            isDone = true;
        }
    } while (isDone == false);
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Error, too many or too few arguments.\n");
        printf("Correct format is ./server YourPort.\n");
        return 1;
    }
    ReadData();
    int sockfd;
    struct sockaddr_in serveraddr, clientaddr;
    int listen_sock, connect_sock;

    struct sockaddr_in server;
    struct sockaddr_in client;
    int sin_size;

    // Step 1: Construct a TCP socket to listen connection request
    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Error in socket.\n");
        return 0;
    }

    // Step 2: Bind address to socket
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = htonl(INADDR_ANY); /* INADDR_ANY puts your IP address automatically */
    if (bind(listen_sock, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        // call bind
        perror("\nError: ");
        return 0;
    }

    // Step 3: Listen request from client
    if (listen(listen_sock, BACKLOG) == -1)
    {
        // call listen
        perror("\nError: ");
        return 0;
    }

    // PrintList()
    int pid;
    // Step 4: Communicate with client
    while (1)
    {
        // accept request
        sin_size = sizeof(struct sockaddr_in);
        if ((connect_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size)) == -1)
            perror("\nError: ");

        printf("You got a connection from %s\n", inet_ntoa(client.sin_addr)); // printf client IP

        pid = fork();
        if (pid == -1) // child
        {
            close(connect_sock);
            printf("Error in new process creation\n");
            exit(1);
        }
        else if (pid > 0) // parent
        {
            // child process
            close(connect_sock);
            continue;
        }
        else if (pid == 0)
        {
            while (1)
            {
                char username[BUFF_SIZE];
                char password[BUFF_SIZE];
                memset(username, 0, strlen(username));
                memset(password, 0, strlen(password));
                printf("Receiving data ...\n");
                // receives username from client
                recv(connect_sock, username, BUFF_SIZE, 0);
                send(connect_sock, "Success username\n", sizeof("Success username\n"), 0);
                username[strlen(username) - 1] = '\0';
                printf("Username: %s\n", username);
                // receives password from client
                recv(connect_sock, password, BUFF_SIZE, 0);
                send(connect_sock, "Password: ", sizeof(" Success password\n "), 0);
                password[strlen(password) - 1] = '\0';
                printf("Password: %s\n", password);
                sleep(1);
                UserSignIn(connect_sock, username, password);
                printf("-------------------------\n");
            }
            close(connect_sock);
            break;
        }
    }
    close(listen_sock);
    return 0;
}
