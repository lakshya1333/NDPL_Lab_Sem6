#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080
#define MAXLINE 1024

// Function to check palindrome
int isPalindrome(char *str) {
    int l = 0, r = strlen(str) - 1;
    while (l < r) {
        if (tolower(str[l]) != tolower(str[r])) return 0;
        l++; r--;
    }
    return 1;
}

// Function to count vowels
void countVowels(char *str, int *vowels) {
    for (int i = 0; i < strlen(str); i++) {
        char c = tolower(str[i]);
        if (c == 'a') vowels[0]++;
        else if (c == 'e') vowels[1]++;
        else if (c == 'i') vowels[2]++;
        else if (c == 'o') vowels[3]++;
        else if (c == 'u') vowels[4]++;
    }
}

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server running on port %d...\n", PORT);

    while (1) {
        len = sizeof(cliaddr);
        int n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';

        if (strcmp(buffer, "Halt") == 0) {
            printf("Termination signal received. Exiting...\n");
            break;
        }

        int vowels[5] = {0};
        countVowels(buffer, vowels);

        char result[MAXLINE];
        snprintf(result, sizeof(result),
                 "Palindrome: %s\nLength: %d\nVowels: A=%d E=%d I=%d O=%d U=%d",
                 isPalindrome(buffer) ? "Yes" : "No",
                 (int)strlen(buffer),
                 vowels[0], vowels[1], vowels[2], vowels[3], vowels[4]);

        sendto(sockfd, result, strlen(result), 0, (struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}
