#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    socklen_t addrlen = sizeof(server_addr);

    char hostname[BUF_SIZE];
    char response[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);
    printf("✅ DNS Server running on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&server_addr, &addrlen);

    recv(client_fd, hostname, BUF_SIZE, 0);

    printf("Client requested domain: %s\n", hostname);

    FILE *fp = fopen("database.txt", "r");

    if (!fp)
    {
        strcpy(response, "Database file not found!");
        send(client_fd, response, BUF_SIZE, 0);
        close(client_fd);
        close(server_fd);
        return 0;
    }

    char domain[BUF_SIZE], ip[BUF_SIZE];
    int found = 0;

    while (fscanf(fp, "%s %s", domain, ip) != EOF)
    {
        if (strcmp(domain, hostname) == 0)
        {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found)
    {
        sprintf(response, "Domain Found!\nIP Address: %s", ip);
    }
    else
    {
        sprintf(response, "Domain Not Found in DNS Database!");
    }

    send(client_fd, response, BUF_SIZE, 0);

    close(client_fd);
    close(server_fd);

    return 0;
}