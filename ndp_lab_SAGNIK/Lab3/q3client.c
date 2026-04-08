#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXSIZE 500

int main() {
    int sockfd;
    struct sockaddr_in serveraddr;
    char buff[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("[ERROR] Connection Failed");
        return 1;
    }

    printf("[SUCCESS] Connected to server.\n");

    while (1) {
        printf("\nEnter alphanumeric string (or 'exit'): ");
        fgets(buff, MAXSIZE, stdin);
        buff[strcspn(buff, "\n")] = 0; // Remove newline

        send(sockfd, buff, strlen(buff), 0);
        if (strcmp(buff, "exit") == 0) break;

        // Receive two results (one from child, one from parent)
        for (int i = 0; i < 2; i++) {
            memset(buff, '\0', sizeof(buff));
            int n = recv(sockfd, buff, sizeof(buff), 0);
            if (n > 0) printf("%s\n", buff);
        }
    }

    close(sockfd);
    return 0;
}