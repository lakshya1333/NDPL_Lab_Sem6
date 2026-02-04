#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];

    if (argc != 2)
    {
        printf("Usage: %s <message>\n", argv[0]);
        exit(0);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    send(sock, argv[1], strlen(argv[1]), 0);

    read(sock, buffer, sizeof(buffer));
    if (strlen(buffer) > 0)
        printf("Server: %s\n", buffer);

    close(sock);
    return 0;
}
