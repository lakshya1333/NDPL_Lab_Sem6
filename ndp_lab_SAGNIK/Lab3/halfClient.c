#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

#define MAXSIZE 1024

main() {
    int sockfd;
    struct sockaddr_in serveraddr;
    char buff[MAXSIZE];
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1) {
        printf("\nSocket Creation Error");
        return;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(1) {
        printf("\nEnter string to permute (or 'stop' to exit): ");
        scanf("%s", buff);

        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

        if (strcmp(buff, "stop") == 0) break;

        printf("Permutations received from server:\n");
        while(1) {
            memset(buff, '\0', MAXSIZE);
            addr_size = sizeof(serveraddr);
            recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, &addr_size);

            // If server sends "DONE", stop listening and ask for next input
            if(strcmp(buff, "DONE") == 0) break;

            printf("%s\n", buff);
        }
    }

    close(sockfd);
}
