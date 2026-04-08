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

#define MAXSIZE 500

main()
{
    char buff[MAXSIZE], str1[MAXSIZE], str2[MAXSIZE];
    int sockfd, retval, choice;
    struct sockaddr_in serveraddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        printf("\nSocket Creation Error");
        return;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if(retval == -1)
    {
        printf("Connection error");
        return;
    }

    printf("Enter filename: ");
    scanf("%s", buff);
    send(sockfd, buff, strlen(buff), 0);

    memset(buff, '\0', sizeof(buff));
    recv(sockfd, buff, sizeof(buff), 0);

    if(strcmp(buff, "File not present") == 0)
    {
        printf("%s\n", buff);
        close(sockfd);
        return;
    }

    while(1)
    {
        printf("\n1. Search\n2. Replace\n3. Reorder\n4. Exit\nEnter choice: ");
        scanf("%d", &choice);
        sprintf(buff, "%d", choice);
        send(sockfd, buff, strlen(buff), 0);

        if(choice == 4) break;

        if(choice == 1)
        {
            printf("Enter string to search: ");
            scanf("%s", str1);
            send(sockfd, str1, strlen(str1), 0);
        }
        else if(choice == 2)
        {
            printf("Enter string to find: ");
            scanf("%s", str1);
            send(sockfd, str1, strlen(str1), 0);
            usleep(1000);
            printf("Enter replacement string: ");
            scanf("%s", str2);
            send(sockfd, str2, strlen(str2), 0);
        }

        memset(buff, '\0', sizeof(buff));
        recv(sockfd, buff, sizeof(buff), 0);
        printf("Server Response: %s\n", buff);
    }
    close(sockfd);
}
