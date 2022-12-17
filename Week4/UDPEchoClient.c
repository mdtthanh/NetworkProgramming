#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Practical.h"

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4) // test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<Server Address/Name> <Echo Word> [<Server Port/Service>]");

    char *server = argv[1];     // First arg: server address/name
    char *echoString = argv[2]; // Second arg: echo
    size_t echoStringLen = strlen(echoString);
    if (echoStringLen > MAXSTRINGLENGTH) // check input length
        DieWithUserMessage(echoString, "string too long");

    // third arg (optional): server port/service
    char *servPort = (argc == 4) ? argv[3] : "echo";

    struct addrinfo addrCriteria;
    memset(&addrCriteria, 0, sizeof(addrCriteria));
    addrCriteria.ai_family = AF_UNSPEC;
    memset(&addrCriteria, 0, sizeof(addrCriteria));

    addrCriteria.ai_socktype = SOCK_DGRAM;
    addrCriteria.ai_protocol = IPPROTO_UDP;

    // Get address
    struct addrinfo *servAddr; // list of server addresses
    int rtnVal = getaddrinfo(server, servPort, &addrCriteria, &servAddr);
    if (rtnVal != 0)
    {
        DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));
    }
    // Create a datagram/UDP socket
    int sock = socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol); // socket descriptor for client

    if (sock < 0)
    {
        DieWithSystemMessage("socket() failed");
    }

    // send the string to the server
    ssize_t numBytes = sendto(sock, echoString, echoStringLen, 0, servAddr->ai_addr, servAddr->ai_addrlen);

    if (numBytes < 0)
    {
        DieWithSystemMessage("sendto() failed");
    }
    else if (numBytes != echoStringLen)
    {
        DieWithUserMessage("sendto() error", "sent unexpected number of bytes");
    }

    // Receive a response
    struct sockaddr_storage fromAddr; // source address of server
    // set length of from address structure
    socklen_t fromAddrLen = sizeof(fromAddr);
    char buffer[MAXSTRINGLENGTH + 1];
    numBytes = recvfrom(sock, buffer, MAXSTRINGLENGTH, 0, (struct sockaddr *)&fromAddr, &fromAddrLen);

    if (numBytes < 0)
    {
        DieWithSystemMessage("recvfrom() failed");
    }
    else if(numBytes != echoStringLen){
        DieWithUserMessage("recvfrom() error", "received unexpected number of bytes");
    }
    

    // verify reception from expected source
    if (!SockAddrsEqual(servAddr->ai_addr, (struct sockaddr *) &fromAddr))
    {
        DieWithUserMessage("recvfrom()", "received a packet from unkown source");
    }

    freeaddrinfo(servAddr);

    buffer[echoStringLen] = '\0';
    printf("Received: %s\n", buffer); // print the echoed string
    close(sock);
    exit(0);
}