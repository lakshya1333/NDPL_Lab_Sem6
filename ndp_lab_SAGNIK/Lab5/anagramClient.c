#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3389
#define MAXSIZE 500

int main() {
    int sockfd;
    struct sockaddr_in serveraddr;
    char s1[MAXSIZE], s2[MAXSIZE], buffer[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Enter first string: ");
    fgets(s1, MAXSIZE, stdin);
    s1[strcspn(s1, "\n")] = 0;

    printf("Enter second string: ");
    fgets(s2, MAXSIZE, stdin);
    s2[strcspn(s2, "\n")] = 0;

    // Sending strings to server
    send(sockfd, s1, strlen(s1), 0);
    usleep(1000); // Tiny delay to ensure server handles separate buffers
    send(sockfd, s2, strlen(s2), 0);

    memset(buffer, 0, MAXSIZE);
    recv(sockfd, buffer, MAXSIZE - 1, 0);
    printf("\n[SERVER RESULT]: %s\n", buffer);

    close(sockfd);
    return 0;
}