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