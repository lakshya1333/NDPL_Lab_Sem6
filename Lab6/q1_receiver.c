#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

int main() {

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char data[100];
    int i, count = 0, parityType;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 3);

    printf("Waiting for sender...\n");

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    read(new_socket, data, 100);

    printf("Received data: %s\n", data);

    printf("Enter parity type used:\n1. Even\n2. Odd\n");
    scanf("%d", &parityType);

    for(i = 0; i < strlen(data); i++) {
        if(data[i] == '1')
            count++;
    }

    if(parityType == 1) {
        if(count % 2 == 0)
            printf("Data received correctly (No Error)\n");
        else
            printf("Data is corrupt (Error detected)\n");
    }
    else {
        if(count % 2 != 0)
            printf("Data received correctly (No Error)\n");
        else
            printf("Data is corrupt (Error detected)\n");
    }

    close(new_socket);
    close(server_fd);

    return 0;
}