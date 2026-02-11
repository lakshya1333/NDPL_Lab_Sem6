#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT); // Convert port number to network byte order

    // Convert IPv4 address from string to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    // Connect
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    printf("Connected to server.\n");

    while (1) {
        printf("\nEnter choice (search/asc/desc/split/exit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; //It removes the newline character \n that gets stored when you use fgets()

        if (strcmp(buffer, "exit") == 0) {
            send(sock, buffer, strlen(buffer), 0);
            break;
        }

        char choice[20];
        strcpy(choice, buffer);

        printf("Enter integers (space separated): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        char message[BUFFER_SIZE];
        sprintf(message, "%s %s", choice, buffer);

        send(sock, message, strlen(message), 0);

        memset(buffer, 0, BUFFER_SIZE);
        read(sock, buffer, BUFFER_SIZE);
        printf("Server response:\n%s\n", buffer);
    }

    close(sock);
    return 0;
}
