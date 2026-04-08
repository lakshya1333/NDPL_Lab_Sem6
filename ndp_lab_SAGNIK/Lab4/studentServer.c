#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define PORT 3388
#define MAXSIZE 500

void search_database(int choice, char* query, char* response) {
    FILE *file = fopen("database.txt", "r");
    if (!file) {
        strcpy(response, "Error: database.txt not found.");
        return;
    }

    char line[MAXSIZE];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char reg[20], name[50], addr[100], dept[20], sem[10], sec[5], sub[20], marks[10];
        // Clean temporary buffers
        memset(reg, 0, 20); memset(name, 0, 50);

        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", 
               reg, name, addr, dept, sem, sec, sub, marks);

        if (choice == 1 && strcmp(reg, query) == 0) {
            sprintf(response, "[CHILD PID %d] Name: %s, Address: %s", getpid(), name, addr);
            found = 1; break;
        } 
        else if (choice == 2 && strcmp(name, query) == 0) {
            sprintf(response, "[CHILD PID %d] Enrollment: %s Dept, Sem %s, Sec %s", getpid(), dept, sem, sec);
            found = 1; break;
        } 
        else if (choice == 3 && strcmp(sub, query) == 0) {
            sprintf(response, "[CHILD PID %d] Subject: %s, Marks: %s", getpid(), sub, marks);
            found = 1; break;
        }
    }

    if (!found) sprintf(response, "[CHILD PID %d] Record not found for: %s", getpid(), query);
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
    printf("[INFO] Server listening on port %d...\n", PORT);

    while (1) {
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);

        while (1) {
            int choice = 0;
            char query[MAXSIZE];
            memset(query, 0, MAXSIZE);

            if (recv(newsockfd, &choice, sizeof(int), 0) <= 0) break;
            
            int n = recv(newsockfd, query, MAXSIZE - 1, 0);
            if (n <= 0) break;
            query[n] = '\0'; // Manually null-terminate!

            if (strcmp(query, "exit") == 0) break;

            if (fork() == 0) {
                char response[MAXSIZE];
                memset(response, 0, MAXSIZE);
                search_database(choice, query, response);
                send(newsockfd, response, strlen(response), 0);
                exit(0);
            }
            waitpid(-1, NULL, WNOHANG);
        }
        close(newsockfd);
    }
    return 0;
}