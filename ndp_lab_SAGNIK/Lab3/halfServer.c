#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

#define MAXSIZE 1024

void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}

// Function to calculate and send all permutations
void findPermutations(char *a, int l, int r, int sockfd, struct sockaddr_in clientaddr) {
    if (l == r) {
        sendto(sockfd, a, strlen(a), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    } else {
        for (int i = l; i <= r; i++) {
            swap((a + l), (a + i));
            findPermutations(a, l + 1, r, sockfd, clientaddr);
            swap((a + l), (a + i)); // backtrack
        }
    }
}

main() {
    int sockfd;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE];
    socklen_t actuallen;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1) {
        printf("\nSocket creation error");
        exit(0);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("Binding error");
        close(sockfd);
        exit(0);
    }

    printf("UDP Server is running and waiting for strings...\n");

    while(1) {
        memset(buff, '\0', MAXSIZE);
        actuallen = sizeof(clientaddr);

        // Receive string from client
        recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&clientaddr, &actuallen);

        if (strcmp(buff, "stop") == 0) break;

        printf("Calculating permutations for: %s\n", buff);

        // Find and send permutations
        int n = strlen(buff);
        findPermutations(buff, 0, n - 1, sockfd, clientaddr);

        // Send an "END" signal to tell the client we are done with permutations
        sendto(sockfd, "DONE", 4, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    }

    close(sockfd);
}
