#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct Request {
    int choice;
    char detail[50];
};

int main() {
    int sock;
    struct sockaddr_in server_addr;
    struct Request req;
    char buffer[2048];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4040);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
        return 1;
    }

    printf("--- Student Information System ---\n");
    printf("1. Search by Registration Number\n");
    printf("2. Search by Student Name\n");
    printf("3. Search by Subject Code\n");
    printf("Enter choice: ");
    scanf("%d", &req.choice);
    getchar();

    printf("Enter the data: ");
    fgets(req.detail, 50, stdin);
    req.detail[strcspn(req.detail, "\n")] = 0;

    send(sock, &req, sizeof(req), 0);

    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    printf("\n[Server Response]:\n%s\n", buffer);

    close(sock);
    return 0;
}