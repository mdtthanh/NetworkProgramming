#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>

// PORT number
#define PORT 5432

int main()
{
    // Server socket id
    int sockfd, ret;

    // server socket address structures
    struct sockaddr_in serverAddr;

    // client socket id
    int clientSocket;

    // client socket address structures
    struct sockaddr_in clientAddr;

    // stores byte size of server socket address
    socklen_t addr_size;

    // child process id
    pid_t childpid;

    // create a TCP socket id from IPV4 family
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Error handing if socket id is not valid
    if (sockfd < 0)
    {
        printf("Error in connection. \n");
        exit(1);
    }

    printf("Server socket is created. \n");

    // Initialize address structure with NULL
    memset(&serverAddr, '\0', sizeof(serverAddr));

    // Assign port number and IP address to the socket created
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    // 127.0.0.1 is a lookback address
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // binding the socket id with the socket structure
    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // Error handling
    if (ret < 0)
    {
        printf("Error in binding. \n");
        exit(1);
    }

    // Listen for connections (upto 10)
    if (listen(sockfd, 10) == 0)
    {
        printf("Listening...\n\n");
    }

    int cnt = 0;

    while (1)
    {
        // Accept client and store their information in cliAddress
        clientSocket = accept(sockfd, (struct sockaddr *)&clientAddr, &addr_size);

        // Error handing
        if (clientSocket < 0)
        {
            exit(1);
        }

        // displaying information of connected client
        printf("Connection accepted from %s: %d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        // print number of clients connected till now
        printf("Client connected: %d\n\n", ++cnt);

        // Creates a child process
        if ((childpid = fork()) == 0)
        {

            // send a confirmation message to the client
            send(clientSocket, "Hello Client!", strlen("Hello Client!"), 0);

            // closing the server socket id
            close(sockfd);
        }
    }

    // close the client socket id
    close(clientSocket);
    return 0;
}