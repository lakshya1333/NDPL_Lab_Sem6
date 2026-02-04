#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_RECORDS 5

struct DNSRecord {
    char hostname[50];
    char ip_address[20];
};

struct DNSRecord db[MAX_RECORDS] = {
    {"google.com", "142.250.190.46"},
    {"facebook.com", "157.240.3.35"},
    {"github.com", "140.82.121.4"},
    {"openai.com", "13.107.246.45"},
    {"manipal.edu", "115.243.160.50"}
};

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char response[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    
    printf("[DNS SERVER] Listening on port %d...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        
        memset(buffer, 0, 1024);
        read(new_socket, buffer, 1024);
        printf("[QUERY] Received request for: %s\n", buffer);

        int found = 0;
        for (int i = 0; i < MAX_RECORDS; i++) {
            if (strcmp(buffer, db[i].hostname) == 0) {
                sprintf(response, "IP Address: %s", db[i].ip_address);
                found = 1;
                break;
            }
        }

        if (!found) {
            strcpy(response, "Error: Hostname not found in database.");
        }

        send(new_socket, response, strlen(response), 0);
        printf("[RESPONSE] Sent result to client.\n");

        close(new_socket);
    }
    return 0;
}