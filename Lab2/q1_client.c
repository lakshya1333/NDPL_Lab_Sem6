#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[BUF_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connect error");
        close(sockfd);
        exit(1);
    }

    char filename[BUF_SIZE];
    printf("Enter filename: ");
    scanf("%s", filename);

    while (1) {
        int choice;
        printf("\nMenu:\n1. Search\n2. Replace\n3. Reorder\n4. Exit\nEnter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            break;
        }

        if (choice == 1) {
            char str[BUF_SIZE];
            printf("Enter string to search: ");
            scanf("%s", str);
            snprintf(buffer, sizeof(buffer), "SEARCH %s %s", filename, str);
        } else if (choice == 2) {
            char str1[BUF_SIZE], str2[BUF_SIZE];
            printf("Enter string to replace: ");
            scanf("%s", str1);
            printf("Enter new string: ");
            scanf("%s", str2);
            snprintf(buffer, sizeof(buffer), "REPLACE %s %s %s", filename, str1, str2);
        } else if (choice == 3) {
            snprintf(buffer, sizeof(buffer), "REORDER %s", filename);
        } else if (choice == 4) {
            snprintf(buffer, sizeof(buffer), "EXIT");
            send(sockfd, buffer, strlen(buffer), 0);
            break;
        } else {
            printf("Invalid choice.\n");
            continue;
        }

        if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
            perror("Send error");
            break;
        }

        memset(buffer, 0, BUF_SIZE);
        int recvLen = recv(sockfd, buffer, BUF_SIZE - 1, 0);
        if (recvLen <= 0) {
            if (recvLen == 0) {
                printf("Server closed connection.\n");
            } else {
                perror("Recv error");
            }
            break;
        }
        buffer[recvLen] = '\0'; // Null-terminate
        printf("Server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
