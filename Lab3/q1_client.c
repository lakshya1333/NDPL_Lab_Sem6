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

    printf("✅ Connected to server!\n");

    printf("\n[CLIENT PARENT] PID = %d, PPID = %d\n", getpid(), getppid());

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        printf("[CLIENT CHILD] PID = %d, PPID = %d\n\n", getpid(), getppid());

        while (1)
        {
            memset(buffer, 0, BUF_SIZE);

            recv(sock, buffer, BUF_SIZE, 0);

            if (strcmp(buffer, "exit") == 0)
            {
                printf("Server exited. Closing chat...\n");
                exit(0);
            }

            printf("Server: %s\n", buffer);
        }
    }

    else
    {
        while (1)
        {
            memset(buffer, 0, BUF_SIZE);

            printf("Client: ");
            fgets(buffer, BUF_SIZE, stdin);

            // Remove newline
            buffer[strcspn(buffer, "\n")] = '\0';

            send(sock, buffer, strlen(buffer), 0);

            if (strcmp(buffer, "exit") == 0)
            {
                printf("Client exiting...\n");
                break;
            }
        }
    }

    close(sock);

    return 0;
}