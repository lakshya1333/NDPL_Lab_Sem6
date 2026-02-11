#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUF_SIZE 1024

/* -------- Function to sort string characters -------- */
void sortString(char str[])
{
    int n = strlen(str);

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (str[i] > str[j])
            {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
}

/* -------- Function to check anagram -------- */
int isAnagram(char str1[], char str2[])
{
    if (strlen(str1) != strlen(str2))
        return 0;

    sortString(str1);
    sortString(str2);

    return strcmp(str1, str2) == 0;
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    char str1[BUF_SIZE], str2[BUF_SIZE], result[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);
    printf("✅ Iterative Server running on port %d...\n", PORT);

    /* ---- Server runs forever for multiple clients ---- */
    while (1)
    {
        client_len = sizeof(client_addr);

        client_fd = accept(server_fd,
                           (struct sockaddr *)&client_addr,
                           &client_len);

        time_t now = time(NULL);

        printf("\n=====================================\n");
        printf("Client Connected!\n");
        printf("Client IP: %s\n", inet_ntoa(client_addr.sin_addr));
        printf("Client Port: %d\n", ntohs(client_addr.sin_port));
        printf("Connected at: %s", ctime(&now));
        printf("=====================================\n");

        memset(str1, 0, BUF_SIZE);
        recv(client_fd, str1, BUF_SIZE, 0);

        memset(str2, 0, BUF_SIZE);
        recv(client_fd, str2, BUF_SIZE, 0);

        printf("Received Strings: '%s' and '%s'\n", str1, str2);

        /* ---- Check Anagram ---- */
        char temp1[BUF_SIZE], temp2[BUF_SIZE];
        strcpy(temp1, str1);
        strcpy(temp2, str2);

        if (isAnagram(temp1, temp2))
            strcpy(result, "✅ Strings are ANAGRAMS.");
        else
            strcpy(result, "❌ Strings are NOT anagrams.");

        send(client_fd, result, BUF_SIZE, 0);

        close(client_fd);
        printf("Client disconnected.\n");
    }

    close(server_fd);
    return 0;
}