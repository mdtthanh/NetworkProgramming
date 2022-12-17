#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "stack.h"
#include <unistd.h>
#define MAXLINE 1024
#define MAX_PORT 49151
#define SA struct sockaddr

void getInput(char *input)
{
    int i = 0;
    char c = getchar();
    while (c != '\n')
    {
        input[i++] = c;
        c = getchar();
    }
    input[i] = '\0';
    fflush(stdin);
}

int convertValue(char *value)
{
    if (strcmp(value, "1") == 0)
    {
        printf("OK\n");
        return ACTIVE;
    }

    if (strcmp(value, "0") == 0 || strcmp(value, "3") == 0)
    {
        printf("Account not ready\n");
        return BLOCKED;
    }
    printf("Not OK\n");
    return WRONG_PASSWORD;
}
int isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}

void func(int sockfd)
{
    char buff[MAXLINE];
    int n;
    for (;;)
    {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0)
        {
            printf("Client Exit...\n");
            break;
        }
    }
}

// Driver code
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Is not enought argument. Type input: ./client IPAddress {{PORT}}\n");
        exit(0);
    }
    // Validdate ip address

    char ipAddress[50];

    strcpy(ipAddress, argv[1]);
    if (!isValidIpAddress(ipAddress))
    {
        printf("Invalid IP address!");
        exit(0);
    }

    // Validate port
    int port = atoi(argv[2]);
    printf("Connecting to %s in port %d\n", ipAddress, port);

    if (port < 0 || port > 65535)
    {
        printf("Port number using must in range 0 -> 65535\n");
        return 0;
    }

    int sockfd;
    char receiveAfterUsername[MAXLINE];

    char *endConnect = "end";

    struct sockaddr_in servaddr;

    fflush(stdin);

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully created..\n");

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    // set to IPv4
    servaddr.sin_family = AF_INET;

    servaddr.sin_port = htons(port);

    // store this IP address in sa:
    inet_pton(AF_INET, ipAddress, &(servaddr.sin_addr));

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    int n;

    char userName[50];
    char password[50];

    while (strcmp(receiveAfterUsername, endConnect) != 0)
    {

        printf("UserName:");
        getInput(userName);

        fflush(stdin);
        write(sockfd, (char *)userName, strlen(userName));
        //    Turn off
        if (strcmp(userName, "") == 0)
        {
            printf("\nBye\n");
            break;
        }
        n = read(sockfd, (char *)receiveAfterUsername, MAXLINE);

        printf("%s\n", receiveAfterUsername);
        printf("Password:");
        getInput(password);
        fflush(stdin);

        write(sockfd, (char *)password, strlen(password));

        char test[50];
        n = read(sockfd, (char *)test, sizeof(test));
        test[n] = '\0';

        // Now going to change password controller
        if (convertValue(test) == ACTIVE)
        {
            char changePassword[100];
            memset(changePassword, '\0', sizeof(changePassword));
            getInput(changePassword);
            fflush(stdin);

            if (strcmp(changePassword, "") == 0)
            {
                printf("End.\n");
                write(sockfd, (char *)changePassword, strlen(changePassword));

                break;
            }

            while (strcmp(changePassword, "") != 1)
            {
                write(sockfd, (char *)changePassword, strlen(changePassword));

                if (strcmp("bye", changePassword) == 0)
                {
                    printf("Goodbye %s\n", userName);
                    break;
                }

                char dataAfterChangePW[50];

                memset(&dataAfterChangePW, '\0', 50);
                read(sockfd, (char *)dataAfterChangePW, MAXLINE);
                if (strcmp(dataAfterChangePW, "Fail") == 0)
                {
                    printf("Error\n");
                }
                else
                {

                    char passwordAlpha[50];
                    char passwordDigit[50];
                    // memset for variable
                    memset(passwordAlpha, '\0', sizeof(passwordAlpha));
                    memset(passwordDigit, '\0', sizeof(passwordDigit));

                    // Check have password alpha?
                    write(sockfd, (char *)"ready", sizeof("ready"));

                    read(sockfd, (char *)passwordAlpha, sizeof(passwordAlpha));
                    printf("passwordAlpha: %s\n", passwordAlpha); //In mỗi token ra

                    write(sockfd, (char *)"hello", sizeof("hello"));
                    n = read(sockfd, (char *)passwordDigit, sizeof(passwordDigit));

                    printf("passwordDigit: %s\n", passwordDigit); //In mỗi token ra

                    // Reset
                    memset(passwordAlpha, 0, sizeof(passwordAlpha));
                    memset(passwordDigit, 0, sizeof(passwordDigit));
                }
                // Reset string
                memset(&changePassword, 0, sizeof(changePassword));
                memset(&dataAfterChangePW, 0, sizeof(dataAfterChangePW));
                scanf("%[^\n]%*c", changePassword);
                if (strcmp(changePassword, "") == 0)
                {
                    write(sockfd, (char *)changePassword, strlen(changePassword));
                    printf("End.\n");
                    break;
                }
            }
        }
    }

    close(sockfd);
    return 0;
}
