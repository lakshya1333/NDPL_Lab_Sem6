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
    socklen_t addr_len = sizeof(server_addr);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8081);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("--- Student Info (UDP) ---\n");
    printf("1. Reg No\n2. Name\n3. Subject Code\nChoice: ");
    scanf("%d", &req.choice);
    getchar();

    printf("Enter data: ");
    fgets(req.detail, 50, stdin);
    req.detail[strcspn(req.detail, "\n")] = 0;

    sendto(sock, &req, sizeof(req), 0, (struct sockaddr*)&server_addr, addr_len);

    memset(buffer, 0, sizeof(buffer));
    recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &addr_len);
    
    printf("\n[UDP Server Response]:\n%s\n", buffer);

    close(sock);
    return 0;
}