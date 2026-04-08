#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#define MAXSIZE 200
main()
{
	char buff[MAXSIZE];
	int sockfd,retval,i;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("\nSocket Creation Error");
		return;
	}
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("Connection error");
		return;
	}
	for (i = 0; ; i+=1)
	{
		memset(buff, '\0', sizeof(buff));
		printf("\n1.Ascending\n2.Descending\n3.Split into even/odd\n4.Exit\nenter choice: ");
		scanf("%s",buff);
		send(sockfd,buff,strlen(buff),0);

		if (strcmp(buff, "4") == 0)
		{
			break;
		}

		printf("Enter space separated integers: ");
		getchar();
		fgets(buff, MAXSIZE, stdin);
		buff[strcspn(buff, "\n")] = 0;
		send(sockfd,buff,strlen(buff),0);

		memset(buff, '\0', sizeof(buff));
		recedbytes=recv(sockfd,buff,sizeof(buff),0);
		printf ("Server Result: %s \n", buff);
	}
	close(sockfd);
}
