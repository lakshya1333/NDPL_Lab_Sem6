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

    char hostname[BUF_SIZE];
    char buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Enter hostname to resolve: ");
    scanf("%s", hostname);

    send(sock, hostname, BUF_SIZE, 0);

    recv(sock, buffer, BUF_SIZE, 0);

    printf("\n--- DNS Response ---\n%s\n", buffer);

    close(sock);

    return 0;
}