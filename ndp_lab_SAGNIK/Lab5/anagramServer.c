#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 3389
#define MAXSIZE 500

// Function to check if two strings are anagrams
int is_anagram(char *str1, char *str2) {
    int count[256] = {0};
    if (strlen(str1) != strlen(str2)) return 0;

    for (int i = 0; str1[i] && str2[i]; i++) {
        count[(unsigned char)str1[i]]++;
        count[(unsigned char)str2[i]]--;
    }

    for (int i = 0; i < 256; i++) {
        if (count[i] != 0) return 0;
    }
    return 1;
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len = sizeof(clientaddr);
    char str1[MAXSIZE], str2[MAXSIZE], response[MAXSIZE];
    int opt = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 5);
    printf("[INFO] Iterative Anagram Server listening on port %d...\n", PORT);

    while (1) {
        // Server handles one client at a time (Iterative)
        newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);

        // Get current Date and Time
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        char time_str[26];
        strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("\n[CONNECTED] Time: %s | Client: %s:%d\n", time_str, client_ip, ntohs(clientaddr.sin_port));

        // Receive first string
        memset(str1, 0, MAXSIZE);
        recv(newsockfd, str1, MAXSIZE, 0);
        
        // Receive second string
        memset(str2, 0, MAXSIZE);
        recv(newsockfd, str2, MAXSIZE, 0);

        printf("[LOG] Received strings: '%s' and '%s'\n", str1, str2);

        if (is_anagram(str1, str2)) {
            strcpy(response, "The strings ARE anagrams.");
        } else {
            strcpy(response, "The strings ARE NOT anagrams.");
        }

        send(newsockfd, response, strlen(response), 0);
        
        close(newsockfd); // Finish connection before accepting the next
        printf("[INFO] Connection closed. Waiting for next client...\n");
    }
    return 0;
}