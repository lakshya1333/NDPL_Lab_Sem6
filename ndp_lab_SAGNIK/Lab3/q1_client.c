#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXSIZE 1024

int main() {
    int sockfd;
    char sendbuff[MAXSIZE], recvbuff[MAXSIZE];
    struct sockaddr_in serveraddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("172.18.175.108");
    serveraddr.sin_port = htons(6767);

    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Connect failed");
        exit(1);
    }
    printf("Connected to server...\n");

    pid_t pid = fork();

    if(pid == 0) { // CHILD PROCESS: Sender
        pid_t my_pid = getpid();
        while(1) {
            bzero(sendbuff, MAXSIZE);
            printf("[CLIENT (PID %d)]> ", my_pid);
            fflush(stdout);
            if (fgets(sendbuff, MAXSIZE, stdin) == NULL) break;
            send(sockfd, sendbuff, strlen(sendbuff) + 1, 0);
        }
    } else { // PARENT PROCESS: Receiver
        pid_t my_pid = getpid();
        while(1) {
            bzero(recvbuff, MAXSIZE);
            int n = recv(sockfd, recvbuff, MAXSIZE, 0);

            if (n > 0) {
                printf("\n[SERVER] > %s", recvbuff);
                printf("[CLIENT (PID %d)]> ", pid);
                fflush(stdout);
            } else {
                printf("\n[INFO] Server shut down. Exiting...\n");
                close(sockfd);
                exit(0);
            }
        }
    }

    return 0;
}
