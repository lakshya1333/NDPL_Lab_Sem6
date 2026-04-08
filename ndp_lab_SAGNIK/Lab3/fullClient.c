#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define MAXSIZE 500

int main() {
    int sockfd;
    struct sockaddr_in serveraddr;
    char buff[MAXSIZE];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("[INFO] Attempting to connect to server...\n");
    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("[ERROR] Connection Failed");
        return 1;
    }

    printf("[SUCCESS] Connected. Parent PID: %d, Child PPID: %d\n", getpid(), getppid());

    childpid = fork();

    if (childpid == 0) {
        // CHILD PROCESS: Receiver
        while (1) {
            memset(buff, '\0', sizeof(buff));
            int n = recv(sockfd, buff, sizeof(buff), 0);
            if (n <= 0) {
                printf("\n[INFO] Server disconnected. Press Enter to exit.\n");
                exit(0);
            }
            // Print received message and reprint the prompt for the parent process
            printf("\n[SERVER]> %s", buff);
            printf("[CLIENT (PID %d)]> ", getppid());
            fflush(stdout);
        }
    } else {
        // PARENT PROCESS: Sender
        while (1) {
            printf("[CLIENT (PID %d)]> ", getpid());
            fflush(stdout);
            memset(buff, '\0', sizeof(buff));

            if (fgets(buff, MAXSIZE, stdin) == NULL) break;

            send(sockfd, buff, strlen(buff), 0);

            if (strncmp(buff, "exit", 4) == 0) {
                kill(childpid, SIGKILL);
                break;
            }
        }
    }

    close(sockfd);
    return 0;
}
