#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// PORT number
#define PORT 5432

int main()
{
    // Socket id
    int clientSocket, ret;

    // client socket structure
    struct sockaddr_in clientAddr;
    struct sockaddr_in serverAddr;

    // char array to store incomming message
    char buffer[1024];

    // Createing socket id
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket < 0)
    {
        printf("Error in connection.\n");
        exit(1);
    }
    printf("Client socket is created.\n");

    // Initializing socket structure with NULL
    memset(&clientAddr, '\0', sizeof(clientAddr));

    // Initializing buffer array with NULL
    memset(buffer, '\0', sizeof(buffer));

    // Assigning port number and IP address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    // 127.0.0.1 is Lockback IP
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // connect() to connect to the server
    ret = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (ret < 0)
    {
        printf("Error in connection.\n");
        exit(1);
    }

    printf("Connected to Server. \n");

    while (1)
    {
        // recv() receives the message
        //  from server and stores in buffer
        if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0)
        {
            printf("Error in receiving data.\n");
        }

        // printing the message on screen
        else
        {
            printf("Server: %s\n", buffer);
            bzero(buffer, sizeof(buffer));
        }
    }
    return 0;
}