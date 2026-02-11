#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t len = sizeof(server_addr);

    int option;
    char input[BUF_SIZE], buffer[BUF_SIZE];


    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    while (1)
    {
        printf("\n====== STUDENT DATABASE MENU ======\n");
        printf("1. Registration Number\n");
        printf("2. Student Name\n");
        printf("3. Subject Code\n");
        printf("4. Exit\n");
        printf("Enter option: ");
        scanf("%d", &option);

        sendto(sockfd, &option, sizeof(option), 0,
               (struct sockaddr *)&server_addr, len);

        if (option == 4)
            break;

        printf("Enter value: ");
        scanf("%s", input);

        sendto(sockfd, input, BUF_SIZE, 0,
               (struct sockaddr *)&server_addr, len);

        memset(buffer, 0, BUF_SIZE);

        recvfrom(sockfd, buffer, BUF_SIZE, 0,
                 (struct sockaddr *)&server_addr, &len);

        printf("\n--- Server Response ---\n%s\n", buffer);
    }

    return 0;
}