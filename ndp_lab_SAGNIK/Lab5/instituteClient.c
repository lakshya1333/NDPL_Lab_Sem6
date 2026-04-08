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
    int id;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Enter Client ID (1 or 2): ");
    scanf("%d", &id);

    if (id == 1) {
        strcpy(buffer, "Institute Of");
    } else if (id == 2) {
        strcpy(buffer, "Technology");
    } else {
        strcpy(buffer, "Unknown");
    }

    // Send the required string
    send(sockfd, buffer, strlen(buffer), 0);
    printf("[SENT] Sent '%s' to server.\n", buffer);

    // Check for termination message from server
    memset(buffer, 0, MAXSIZE);
    int n = recv(sockfd, buffer, MAXSIZE - 1, 0);
    if (n > 0) {
        buffer[n] = '\0';
        if (strcmp(buffer, "terminate session") == 0) {
            printf("[SERVER]: %s\n", buffer);
            close(sockfd);
            exit(0);
        }
    }

    close(sockfd);
    return 0;
}