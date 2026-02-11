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
    char filename[BUF_SIZE], buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1)
    {
        printf("\nEnter filename (or stop): ");
        scanf("%s", filename);

        send(sock, filename, BUF_SIZE, 0);

        if (strcmp(filename, "stop") == 0)
            break;

        recv(sock, buffer, BUF_SIZE, 0);

        if (strcmp(buffer, "File not found!") == 0)
        {
            printf("❌ %s\n", buffer);
            continue;
        }

        printf("\n✅ %s\n", buffer);

        // Receive stats
        recv(sock, buffer, BUF_SIZE, 0);
        printf("\n--- File Statistics ---\n%s\n", buffer);

        printf("--- File Content ---\n");

        while (1)
        {
            recv(sock, buffer, BUF_SIZE, 0);

            if (strcmp(buffer, "EOF") == 0)
                break;

            printf("%s", buffer);
        }

        printf("\n----------------------\n");
    }

    close(sock);
    return 0;
}