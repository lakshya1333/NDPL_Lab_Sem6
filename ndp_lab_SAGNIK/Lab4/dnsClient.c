#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3388
#define MAXSIZE 500

int main() {
    int sockfd;
    struct sockaddr_in serveraddr;
    char buffer[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Connection to DNS Server failed");
        return 1;
    }

    printf("Connected to DNS Server. Type 'exit' to quit.\n");

    while (1) {
        printf("\nEnter Domain or IP to lookup: ");
        fgets(buffer, MAXSIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline

        send(sockfd, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit") == 0) break;

        memset(buffer, 0, MAXSIZE);
        int n = recv(sockfd, buffer, MAXSIZE - 1, 0);
        if (n > 0) {
            buffer[n] = '\0';
            printf("[DNS RESPONSE]: %s\n", buffer);
        } else {
            printf("Server disconnected.\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}