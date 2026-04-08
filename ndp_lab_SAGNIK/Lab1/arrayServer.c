#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#define MAXSIZE 500

main()
{
	int sockfd,newsockfd,retval,i,j,k;
	socklen_t actuallen;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	char buff[MAXSIZE];
	char result[MAXSIZE];
	int arr[100];

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("\nSocket creation error");
	}
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		close(sockfd);
		return;
	}
	listen(sockfd,1);
	actuallen=sizeof(clientaddr);
	newsockfd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);

	for (i = 0; ; i+=1)
	{
		memset(buff, '\0', sizeof(buff));
		recedbytes=recv(newsockfd,buff,sizeof(buff),0);
		if(recedbytes<=0 || strcmp(buff, "4") == 0)
		{
			break;
		}
		int choice = atoi(buff);

		memset(buff, '\0', sizeof(buff));
		recedbytes=recv(newsockfd,buff,sizeof(buff),0);

		printf("--- Iteration Information ---\n");
		printf("Received Choice: %d\n", choice);
		printf("Received Initial Array String: %s\n", buff);

		int count = 0;
		char *token = strtok(buff, " ");
		while (token != NULL)
		{
			arr[count++] = atoi(token);
			token = strtok(NULL, " ");
		}

		memset(result, '\0', sizeof(result));
		char temp[20];

		switch(choice)
		{
			case 1:
				for(j=0; j<count-1; j++)
					for(k=0; k<count-j-1; k++)
						if(arr[k] > arr[k+1]) {
							int t = arr[k];
							arr[k] = arr[k+1];
							arr[k+1] = t;
						}
				for(j=0; j<count; j++) {
					sprintf(temp, "%d ", arr[j]);
					strcat(result, temp);
				}
				break;

			case 2:
				for(j=0; j<count-1; j++)
					for(k=0; k<count-j-1; k++)
						if(arr[k] < arr[k+1]) {
							int t = arr[k];
							arr[k] = arr[k+1];
							arr[k+1] = t;
						}
				for(j=0; j<count; j++) {
					sprintf(temp, "%d ", arr[j]);
					strcat(result, temp);
				}
				break;

			case 3:
				strcat(result, "Even: ");
				for(j=0; j<count; j++)
					if(arr[j]%2==0) {
						sprintf(temp, "%d ", arr[j]);
						strcat(result, temp);
					}
				strcat(result, "| Odd: ");
				for(j=0; j<count; j++)
					if(arr[j]%2!=0) {
						sprintf(temp, "%d ", arr[j]);
						strcat(result, temp);
					}
				break;

			default:
				strcpy(result, "Invalid Option");
		}
		send(newsockfd,result,strlen(result),0);
	}
	close(newsockfd);
	close(sockfd);
}
