#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 50

int main()
{
    char buff[MAXSIZE];
    int sockfd;
    struct sockaddr_in serveraddr;
    pid_t pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Socket error");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("172.18.175.109");

    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("Connection error");
        close(sockfd);
        exit(1);
    }

    printf("Connected to server\n");

    pid = fork();

    /* CHILD PROCESS → RECEIVE */
    if (pid == 0)
    {
        while (1)
        {
            memset(buff, 0, MAXSIZE);
            if (recv(sockfd, buff, MAXSIZE, 0) <= 0)
                break;

            printf("Server: %s\n", buff);

            if (strncmp(buff, "stop", 4) == 0)
                break;
        }
        exit(0);
    }

    /* PARENT PROCESS → SEND */
    else
    {
        while (1)
        {
            memset(buff, 0, MAXSIZE);
            printf("Client: ");
            scanf("%s", buff);

            send(sockfd, buff, strlen(buff), 0);

            if (strncmp(buff, "stop", 4) == 0)
                break;
        }
    }

    close(sockfd);
    return 0;
}
