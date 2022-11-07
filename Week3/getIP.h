
void getHostnamegetIP(char string[])
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