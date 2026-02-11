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

    char str1[BUF_SIZE], str2[BUF_SIZE], buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("✅ Connected to server!\n");

    printf("Enter first string: ");
    scanf("%s", str1);

    printf("Enter second string: ");
    scanf("%s", str2);

    send(sock, str1, BUF_SIZE, 0);
    send(sock, str2, BUF_SIZE, 0);

    recv(sock, buffer, BUF_SIZE, 0);

    printf("\n--- Server Response ---\n%s\n", buffer);

    close(sock);

    return 0;
}