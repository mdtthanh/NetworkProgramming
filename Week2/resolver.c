#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void getIP(char string[]);
void getHostname(char string[]);

int main(int argc, char const *argv[])
{
	char string[30];

	strcpy(string, argv[2]);

	if (argc != 3)
	{
		printf("Wrong parameter.\n");
	}

	if (strcmp(argv[1], "2") == 0)
	{ /* string is a name */
		getIP(string);
	}
	else if (strcmp(argv[1], "1") == 0)
	{
		getHostname(string); /* string is an ip address */
	}

	else
	{
		printf("Wrong parameter.\n");
	}
}

void getIP(char string[])
{
	struct hostent *hostname;
	printf("Hostname: %s\n", string);
	hostname = gethostbyname(string);

	if (hostname == NULL)
	{
		printf("Not found information.\n");
		return;
	}
	printf("Offical IP: %s \n", inet_ntoa(*((struct in_addr *)hostname->h_addr)));

	printf("Alias IP:\n");
	struct in_addr **addr_list = (struct in_addr **)hostname->h_addr_list;

	for (int i = 1; addr_list[i] != NULL; i++)
	{
		printf("%s\n", inet_ntoa(*addr_list[i]));
	}
}

void getHostname(char string[])
{
	struct hostent *hostIP;
	struct in_addr address;

	printf("IP Address: %s\n", string);
	address.s_addr = inet_addr(string);
	if (address.s_addr == INADDR_NONE)
	{
		printf("Not found information.\n");
		return;
	}
	else
	{
		hostIP = gethostbyaddr((char *)&address, 4, AF_INET);
	}

	if (hostIP == NULL)
	{
		printf("Not found information.\n");
		return;
	}

	printf("Offical name: %s\n", hostIP->h_name);
	char **pAlias;

	printf("Alias name: \n");

	for (pAlias = hostIP->h_aliases; *pAlias != NULL; pAlias++)
	{
		printf("%s\n", *pAlias);
	}
}