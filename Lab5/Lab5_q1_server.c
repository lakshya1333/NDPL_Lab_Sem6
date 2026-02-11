#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 2

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address, client_addr;
    int addrlen = sizeof(address);
    int client_count = 0;

    char final_output[256] = "";
    char file_content[50] = "";

    // 1. Read "Manipal" from file initially
    FILE *fp = fopen("data.txt", "r");
    if (fp == NULL) {
        perror("File opening failed");
        return 1;
    }
    fgets(file_content, sizeof(file_content), fp);
    fclose(fp);
    
    // Remove newline if present
    file_content[strcspn(file_content, "\n")] = 0;
    strcpy(final_output, file_content);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
        client_count++;

        if (client_count > MAX_CLIENTS) {
            printf("Client 3 tried to connect. Terminating session.\n");
            send(new_socket, "terminate session", 17, 0);
            close(new_socket);
            close(server_fd);
            exit(0);
        }

        // To keep data in one place, we handle the string in the parent 
        // before/after the fork or skip fork for the string logic
        char buffer[1024] = {0};
        int valread = read(new_socket, buffer, sizeof(buffer));
        buffer[valread] = '\0'; // Safety null terminator

        printf("Client %d (%s:%d) sent: %s\n", 
                client_count, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

        // Concatenate the string
        strcat(final_output, " ");
        strcat(final_output, buffer);

        if (client_count == MAX_CLIENTS) {
            printf("\nFinal Output:\n%s\n", final_output);
        }

        if (fork() == 0) {
            // In a real concurrent server, the child would do the work here
            close(new_socket);
            exit(0);
        }
    }
    return 0;
}
