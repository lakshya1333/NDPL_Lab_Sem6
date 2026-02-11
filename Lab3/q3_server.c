// 3. Write two separate C programs (one for server and other for client) using socket
// APIs, to implement the following connection-oriented client-server model.
// (a) The user at the client side sends an alphanumeric string to the server.
// (b) The child process at the server sorts the numbers of the alphanumeric string
// in ascending order. The parent process at the server sorts the characters of the
// alphanumeric string in descending order.
// (c) Both the processes send the results to the client along with its corresponding
// process ID.
// Sample Output: At the client side:
// Input string: hello451bye7324
// At the server side:
// Output at the child process of the server: 1234457
// Output at the parent process of the server: yollheeb


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

void sortAscending(char str[])
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

void sortDescending(char str[])
{
    int n = strlen(str);
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (str[i] < str[j])
            {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    char input[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));

    listen(server_fd, 5);
    printf("✅ Server listening on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);

    recv(client_fd, input, BUF_SIZE, 0);

    printf("\nInput received from client: %s\n", input);

    pid_t pid = fork();

    if (pid == 0)
    {
        char digits[BUF_SIZE] = "";
        int k = 0;

        for (int i = 0; i < strlen(input); i++)
        {
            if (input[i] >= '0' && input[i] <= '9')
                digits[k++] = input[i];
        }
        digits[k] = '\0';

        sortAscending(digits);

        char msg[BUF_SIZE];
        sprintf(msg,
                "Output at Child Process (PID=%d): %s",
                getpid(), digits);

        send(client_fd, msg, strlen(msg), 0);

        exit(0);
    }

    else
    {
        char letters[BUF_SIZE] = "";
        int k = 0;

        for (int i = 0; i < strlen(input); i++)
        {
            if ((input[i] >= 'A' && input[i] <= 'Z') ||
                (input[i] >= 'a' && input[i] <= 'z'))
                letters[k++] = input[i];
        }
        letters[k] = '\0';

        sortDescending(letters);

        char msg[BUF_SIZE];
        sprintf(msg,
                "Output at Parent Process (PID=%d): %s",
                getpid(), letters);

        send(client_fd, msg, strlen(msg), 0);

        wait(NULL);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}