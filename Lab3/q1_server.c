//  Write two separate C programs using UNIX socket APIs illustrate full duplex mode
// chat application between a single client and server using connection oriented service.
// Display PID and PPID of both parent and child processes

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
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);

    char buffer[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);
    printf("✅ Server listening on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
    printf("✅ Client connected!\n");

    /* -------- DISPLAY SERVER PID -------- */
    printf("\n[SERVER PARENT] PID = %d, PPID = %d\n", getpid(), getppid());

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        printf("[SERVER CHILD] PID = %d, PPID = %d\n\n", getpid(), getppid());

        while (1)
        {
            memset(buffer, 0, BUF_SIZE);

            // Receive message from client
            recv(client_fd, buffer, BUF_SIZE, 0);

            if (strcmp(buffer, "exit") == 0)
            {
                printf("Client exited. Closing chat...\n");
                exit(0);
            }

            printf("Client: %s\n", buffer);
        }
    }

    else
    {
        while (1)
        {
            memset(buffer, 0, BUF_SIZE);

            printf("Server: ");
            fgets(buffer, BUF_SIZE, stdin);

            // Remove newline
            buffer[strcspn(buffer, "\n")] = '\0';

            send(client_fd, buffer, strlen(buffer), 0);

            if (strcmp(buffer, "exit") == 0)
            {
                printf("Server exiting...\n");
                break;
            }
        }
    }

    close(client_fd);
    close(server_fd);

    return 0;
}