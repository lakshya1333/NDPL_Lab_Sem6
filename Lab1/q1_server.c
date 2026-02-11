// Write two separate C programs (one for server and other for client) using socket
// APIs for TCP, to implement the client-server model such that the client should send
// a set of integers along with a choice to search for a number or sort the given set
// in ascending/descending order or split the given set to odd and even to the server.
// The server performs the relevant operation according to the choice. Client should
// continue to send the messages until the user enters selects the choice “exit”.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> // Socket structures and functions

#define PORT 8080
#define BUFFER_SIZE 1024

void sortArray(int arr[], int n, int ascending) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((ascending && arr[i] > arr[j]) || (!ascending && arr[i] < arr[j])) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}


void splitOddEven(int arr[], int n, char *result) {
    char odd[BUFFER_SIZE] = "", even[BUFFER_SIZE] = "";
    for (int i = 0; i < n; i++) {
        char temp[20];
        sprintf(temp, "%d ", arr[i]);
        if (arr[i] % 2 == 0)
            strcat(even, temp);
        else
            strcat(odd, temp);
    }
    sprintf(result, "Odd: %s\nEven: %s\n", odd, even);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE]; // Buffer to store client message

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); // Convert port to network byte order
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);

    // Accept connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);  // Clear buffer before reading new message
        read(new_socket, buffer, BUFFER_SIZE);

        if (strcmp(buffer, "exit") == 0) {
            printf("Client requested exit. Closing connection.\n");
            break;
        }

        // Parse choice and numbers
        char choice[20];
        int arr[100], n = 0, searchNum;
        sscanf(buffer, "%s", choice); // Extract the first word (choice)

        char *token = strtok(buffer + strlen(choice), " "); // Tokenize remaining string into integers
        while (token != NULL) {
            arr[n++] = atoi(token); //converts string to integer
            token = strtok(NULL, " ");
        }

        char result[BUFFER_SIZE] = "";

        if (strcmp(choice, "search") == 0) {
            searchNum = arr[n-1]; // last number is search target
            int found = 0;
            for (int i = 0; i < n-1; i++) {
                if (arr[i] == searchNum) {
                    found = 1;
                    break;
                }
            }
            sprintf(result, "Search result: %d %s\n", searchNum, found ? "found" : "not found");
        } else if (strcmp(choice, "asc") == 0) {
            sortArray(arr, n, 1);
            sprintf(result, "Sorted ascending: ");
            for (int i = 0; i < n; i++) {
                char temp[20];
                sprintf(temp, "%d ", arr[i]);
                strcat(result, temp);
            }
        } else if (strcmp(choice, "desc") == 0) {
            sortArray(arr, n, 0);
            sprintf(result, "Sorted descending: ");
            for (int i = 0; i < n; i++) {
                char temp[20];
                sprintf(temp, "%d ", arr[i]);
                strcat(result, temp);
            }
        } else if (strcmp(choice, "split") == 0) {
            splitOddEven(arr, n, result);
        } else {
            sprintf(result, "Invalid choice.\n");
        }

        send(new_socket, result, strlen(result), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
