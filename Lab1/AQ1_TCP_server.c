// Write two separate C programs (one for the Server and the other for Client) using
// UNIX socket APIs using both connection oriented and connectionless services, in
// which the server displays the client’s socket address, IP address and port number on
// the server screen.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    printf("✅ TCP Server socket created.\n");


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("✅ TCP Server bound to port %d.\n", PORT);


    listen(server_fd, 5);
    printf("✅ Waiting for client connection...\n");

    client_len = sizeof(client_addr);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

    if (client_fd < 0)
    {
        perror("Accept failed");
        exit(1);
    }


    printf("\n===== Client Details (TCP) =====\n");

    printf("Client Socket Address: %u\n", client_addr.sin_addr.s_addr);

    printf("Client IP Address: %s\n", inet_ntoa(client_addr.sin_addr));

    printf("Client Port Number: %d\n", ntohs(client_addr.sin_port));

    printf("================================\n");

    close(client_fd);
    close(server_fd);

    return 0;
}
