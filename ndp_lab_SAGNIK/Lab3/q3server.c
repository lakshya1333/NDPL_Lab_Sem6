#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>

#define MAXSIZE 500

// Helper to sort chars: ascending if mode=1, descending if mode=0
void sort_string(char* s, int len, int ascending) {
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (ascending ? (s[i] > s[j]) : (s[i] < s[j])) {
                char temp = s[i];
                s[i] = s[j];
                s[j] = temp;
            }
        }
    }
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t actuallen = sizeof(clientaddr);
    char buff[MAXSIZE], result[MAXSIZE + 50];
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
    printf("[INFO] Server listening on port 3388...\n");

    newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &actuallen);
    
    while (1) {
        memset(buff, '\0', sizeof(buff));
        if (recv(newsockfd, buff, sizeof(buff), 0) <= 0) break;
        
        if (strncmp(buff, "exit", 4) == 0) break;

        pid_t pid = fork();

        if (pid == 0) { 
            // CHILD: Sort numbers ascending
            char nums[MAXSIZE];
            int k = 0;
            for (int i = 0; buff[i]; i++) {
                if (isdigit(buff[i])) nums[k++] = buff[i];
            }
            nums[k] = '\0';
            sort_string(nums, k, 1);
            sprintf(result, "[CHILD PID %d] Sorted Numbers: %s", getpid(), nums);
            send(newsockfd, result, strlen(result), 0);
            exit(0); // Child finishes after sending
        } else {
            // PARENT: Sort characters descending
            char alpha[MAXSIZE];
            int k = 0;
            for (int i = 0; buff[i]; i++) {
                if (isalpha(buff[i])) alpha[k++] = buff[i];
            }
            alpha[k] = '\0';
            sort_string(alpha, k, 0);
            sprintf(result, "[PARENT PID %d] Sorted Chars: %s", getpid(), alpha);
            send(newsockfd, result, strlen(result), 0);
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}