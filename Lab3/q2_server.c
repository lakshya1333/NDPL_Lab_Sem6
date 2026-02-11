// Write two separate C programs using UNIX socket APIs illustrate half duplex mode
// chat application between a single client and server connection less service in which
// the server estimates and prints all permutations of a string sent by the client.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 1024

void swap(char *x, char *y)
{
    char temp = *x;
    *x = *y;
    *y = temp;
}

// void permute(char *str, int left, int right,
//              int sockfd, struct sockaddr_in client_addr, socklen_t len)
void permute(char *str, int left, int right)
{
    if (left == right)
    {
        // // Send permutation to client
        // sendto(sockfd, str, strlen(str), 0,
        //        (struct sockaddr *)&client_addr, len);
        // return;
        printf("%s\n", str); // Print one permutation
        return;
    }

    for (int i = left; i <= right; i++)
    {
        swap(&str[left], &str[i]);
        permute(str, left + 1, right);
        swap(&str[left], &str[i]); // Backtrack
    }
}

int main()
{
    int sockfd;
    char buffer[BUF_SIZE];

    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("✅ UDP Half Duplex Server running on port %d...\n", PORT);

    while (1)
    {
        memset(buffer, 0, BUF_SIZE);

        recvfrom(sockfd, buffer, BUF_SIZE, 0,
                 (struct sockaddr *)&client_addr, &len);

        if (strcmp(buffer, "exit") == 0)
        {
            printf("Client exited.\n");
            break;
        }

        printf("\nString received from client: %s\n", buffer);

        printf("Permutations are:\n");

        int n = strlen(buffer);
        permute(buffer, 0, n - 1);

        char msg[] = "Permutations printed on server side!";
        sendto(sockfd, msg, strlen(msg), 0,
               (struct sockaddr *)&client_addr, len);
    }

    return 0;
}