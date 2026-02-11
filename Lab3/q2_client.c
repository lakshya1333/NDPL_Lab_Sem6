#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 1024

int main()
{
    int sockfd;
    char buffer[BUF_SIZE];

    struct sockaddr_in server_addr;
    socklen_t len = sizeof(server_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    while (1)
    {
        printf("\nEnter a string (or exit): ");
        fgets(buffer, BUF_SIZE, stdin);

        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline

        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server_addr, len);

        if (strcmp(buffer, "exit") == 0)
            break;

        memset(buffer, 0, BUF_SIZE);

        recvfrom(sockfd, buffer, BUF_SIZE, 0,
                 (struct sockaddr *)&server_addr, &len);

        printf("Server reply: %s\n", buffer);
    }

    return 0;
}