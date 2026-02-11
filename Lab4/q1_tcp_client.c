#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in server_addr;

    int option;
    char input[BUF_SIZE], buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while (1)
    {
        printf("\n====== STUDENT DATABASE MENU ======\n");
        printf("1. Registration Number\n");
        printf("2. Student Name\n");
        printf("3. Subject Code\n");
        printf("4. Exit\n");
        printf("Enter option: ");
        scanf("%d", &option);

        send(sock, &option, sizeof(option), 0);

        if (option == 4)
            break;

        printf("Enter value: ");
        scanf("%s", input);

        send(sock, input, BUF_SIZE, 0);
        memset(buffer, 0, BUF_SIZE);
        recv(sock, buffer, BUF_SIZE, 0);

        printf("\n--- Server Response ---\n%s\n", buffer);
    }

    close(sock);
    return 0;
}