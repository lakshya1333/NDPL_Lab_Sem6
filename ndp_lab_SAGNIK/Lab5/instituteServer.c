#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 3388
#define MAXSIZE 500

void handle_client_logic(int newsockfd, struct sockaddr_in addr, int client_num) {
    char buffer[MAXSIZE];
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    int port = ntohs(addr.sin_port);

    // Receive string from client
    int n = recv(newsockfd, buffer, MAXSIZE - 1, 0);
    if (n <= 0) return;
    buffer[n] = '\0';

    printf("[LOG] Received '%s' from %s:%d\n", buffer, client_ip, port);

    // File manipulation logic
    FILE *fp = fopen("institute.txt", "a+");
    if (fp) {
        fprintf(fp, " %s", buffer);
        fclose(fp);
    }

    // Read the final combined string to display
    fp = fopen("institute.txt", "r");
    char final_str[MAXSIZE];
    fgets(final_str, MAXSIZE, fp);
    fclose(fp);

    printf("[RESULT] %s (Client %d: %s:%d)\n", final_str, client_num, client_ip, port);
}

int main() {
    int sockfd, newsockfd, client_count = 0;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len = sizeof(clientaddr);
    int opt = 1;

    // Initialize file with "Manipal"
    FILE *fp = fopen("institute.txt", "w");
    fprintf(fp, "Manipal");
    fclose(fp);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 5);
    printf("[INFO] Server listening on port %d...\n", PORT);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
        client_count++;

        if (client_count > 2) {
            char *msg = "terminate session";
            printf("[CRITICAL] Client limit exceeded. Sending termination.\n");
            // In a real scenario, you'd broadcast to all. Here we notify the 3rd and exit.
            send(newsockfd, msg, strlen(msg), 0);
            close(newsockfd);
            close(sockfd);
            exit(0); 
        }

        if (fork() == 0) { // Child process
            handle_client_logic(newsockfd, clientaddr, client_count);
            close(newsockfd);
            exit(0);
        }
        
        waitpid(-1, NULL, WNOHANG); // Clean up zombies
    }
    return 0;
}