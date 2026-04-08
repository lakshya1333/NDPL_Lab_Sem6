#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define PORT 3388
#define MAXSIZE 500

void lookup_dns(char* query, char* response) {
    FILE *file = fopen("dns.txt", "r");
    if (!file) {
        strcpy(response, "Error: DNS Database not found.");
        return;
    }

    char domain[100], ip[100];
    int found = 0;

    while (fscanf(file, "%s %s", domain, ip) != EOF) {
        if (strcmp(domain, query) == 0) {
            sprintf(response, "IP Address: %s", ip);
            found = 1; break;
        } else if (strcmp(ip, query) == 0) {
            sprintf(response, "Domain Name: %s", domain);
            found = 1; break;
        }
    }

    if (!found) strcpy(response, "404: Not Found in DNS Records.");
    fclose(file);
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr;
    int opt = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 5);

    printf("[DNS SERVER] Listening on port %d...\n", PORT);

    while (1) {
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);

        while (1) {
            char query[MAXSIZE], response[MAXSIZE];
            memset(query, 0, MAXSIZE);

            int n = recv(newsockfd, query, MAXSIZE - 1, 0);
            if (n <= 0) break; 
            query[n] = '\0';

            if (strcmp(query, "exit") == 0) break;

            if (fork() == 0) { 
                memset(response, 0, MAXSIZE);
                lookup_dns(query, response);
                send(newsockfd, response, strlen(response), 0);
                exit(0);
            }
            waitpid(-1, NULL, WNOHANG);
        }
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}