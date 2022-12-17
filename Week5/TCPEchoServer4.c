#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <Practical.h>

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int main(int argc, char *argv[])
{
    if (argc != 2) // test for correct number of arguments
        DieWithSystemMessage("Parameter", "<Server Port>");

    in_port_t servPort = atoi(argv[1]); // first arg: local port

    // create socket for incoming connections
    int servSock; // socket descriptor for server
    if (servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) < 0)
        DieWithSystemMessage("socket() failed");

    // construct local address structure
    struct sockaddr_in servAddr;                  // local address
    memset(&servAddr, 0, sizeof(servAddr));       // zero out structure
    servAddr.sin_family = AF_INET;                // IPv4 address family
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // any incoming interface
    servAddr.sin_port = htons(servPort);          // local port

    // bind to the local address
    if (bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("bind() failed");

    // mark the socket so it will listen for incoming connections
    if (listen(servSock, MAXPENDING) < 0)
        DieWithSystemMessage("listen() failed");

    for (;;) // run forever
    {
        struct sockaddr_in clntAddr; // client address
        // set length of client address structure (in-out parameter)
        socklent_t clnAddrLen = sizeof(clntAddr);

        // Wait for a client to connect
        int clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntAddrLen);
        if (clntSock < 0)
            DieWithSystemMessage("accept() failed");

        // clntSock is connected to a client!

        char clntName[INET_ADDRSTRLEN]; // string to contain client address
        if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
            printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
        else
            puts("Unable to get client address");
        HandleTCPClient(clntSock);
    }
    // not reached 
}