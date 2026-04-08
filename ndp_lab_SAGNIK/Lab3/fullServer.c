#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>

#define MAXSIZE 500

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t actuallen = sizeof(clientaddr);
    char buff[MAXSIZE];
    pid_t childpid;
    int opt = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("[ERROR] Bind failed");
        exit(1);
    }

    listen(sockfd, 1);
    printf("[INFO] Server listening on port...\n");

    newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &actuallen);
    printf("[SUCCESS] Client connected. PID: %d\n", getpid());

    childpid = fork();

    if (childpid == 0) {
        // CHILD: Receiver
        while (1) {
            memset(buff, '\0', sizeof(buff));
            if (recv(newsockfd, buff, sizeof(buff), 0) <= 0) {
                printf("\n[INFO] Client disconnected.\n");
                exit(0);
            }
            printf("\n[CLIENT]> %s", buff);
            printf("[SERVER (PID %d)]> ", getppid());
            fflush(stdout);
        }
    } else {
        // PARENT: Sender
        while (1) {
            printf("[SERVER (PID %d)]> ", getpid());
            fflush(stdout);
            memset(buff, '\0', sizeof(buff));
            fgets(buff, MAXSIZE, stdin);
            send(newsockfd, buff, strlen(buff), 0);
            if (strncmp(buff, "exit", 4) == 0) {
                kill(childpid, SIGKILL);
                break;
            }
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
