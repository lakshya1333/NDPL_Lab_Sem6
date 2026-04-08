#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3388
#define MAXSIZE 500

int main() {
    int sockfd;
    struct sockaddr_in serveraddr;
    char query[MAXSIZE], buffer[MAXSIZE];
    int choice;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    while (1) {
        printf("\n--- SEARCH MENU ---");
        printf("\n1. Registration Number\n2. Student Name\n3. Subject Code");
        printf("\nEnter choice (0 to exit): ");
        fflush(stdout);
        
        if (scanf("%d", &choice) != 1) break;
        getchar(); // clear newline

        if (choice == 0) {
            strcpy(query, "exit");
            send(sockfd, &choice, sizeof(int), 0);
            send(sockfd, query, strlen(query), 0);
            break;
        }

        printf("Enter search value: ");
        fflush(stdout);
        fgets(query, MAXSIZE, stdin);
        query[strcspn(query, "\n")] = 0; // Remove \n

        send(sockfd, &choice, sizeof(int), 0);
        send(sockfd, query, strlen(query), 0);

        // Clear buffer before receiving
        memset(buffer, 0, MAXSIZE);
        int n = recv(sockfd, buffer, MAXSIZE - 1, 0);
        
        if (n > 0) {
            buffer[n] = '\0'; // Null terminate received data
            printf("\n[SERVER]: %s\n", buffer);
        } else {
            break;
        }
    }

    printf("\nExiting...\n");
    close(sockfd);
    return 0;
}