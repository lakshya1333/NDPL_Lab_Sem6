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
    struct sockaddr_in serv_addr;

    char input[BUF_SIZE], buffer[BUF_SIZE];
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Enter alphanumeric string: ");
    scanf("%s", input);

    send(sock, input, strlen(input), 0);

    memset(buffer, 0, BUF_SIZE);
    recv(sock, buffer, BUF_SIZE, 0);
    printf("\n%s\n", buffer);

    memset(buffer, 0, BUF_SIZE);
    recv(sock, buffer, BUF_SIZE, 0);
    printf("%s\n", buffer);

    close(sock);

    return 0;
}