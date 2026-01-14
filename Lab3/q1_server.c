#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXBUF 1024

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[MAXBUF];
    pid_t pid;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Bind socket
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error on binding");
        exit(1);
    }

    // Listen
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("Server waiting for connection...\n");
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("Error on accept");
        exit(1);
    }

    printf("Client connected.\n");

    // Fork for full-duplex
    pid = fork();
    if (pid < 0) {
        perror("Error on fork");
        exit(1);
    }

    if (pid == 0) {
        // Child process: send messages
        printf("Child Process (PID=%d, PPID=%d)\n", getpid(), getppid());
        while (1) {
            memset(buffer, 0, MAXBUF);
            fgets(buffer, MAXBUF, stdin);
            write(newsockfd, buffer, strlen(buffer));
        }
    } else {
        // Parent process: receive messages
        printf("Parent Process (PID=%d, PPID=%d)\n", getpid(), getppid());
        while (1) {
            memset(buffer, 0, MAXBUF);
            int n = read(newsockfd, buffer, MAXBUF);
            if (n > 0) {
                printf("Client: %s", buffer);
            }
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
