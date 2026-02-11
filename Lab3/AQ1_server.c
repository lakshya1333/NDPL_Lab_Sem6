// Write a C program to simulate a menu driven calculator using client server
// architecture that performs the following. The client prompts the user with the
// options as listed below 1. Add/Subtract two integers
// 2. Find the value of ’x’ in a linear equation
// 3. Multiply two matrices
// 4. Exit
// Based on the user input the client prompts the user to enter required data. The client
// sends the option chosen and the relevant data to the server. The server performs the
// required operation and sends the result to the client. Note that if option 1 is selected,
// the server provides result of both addition and subtraction of the two integers.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 10

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    int choice;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));

    listen(server_fd, 5);
    printf("✅ Calculator Server running on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);
    printf("✅ Client Connected!\n");

    while (1)
    {
        /* Receive choice from client */
        recv(client_fd, &choice, sizeof(choice), 0);

        if (choice == 4)
        {
            printf("Client exited.\n");
            break;
        }

        if (choice == 1)
        {
            int a, b;
            recv(client_fd, &a, sizeof(a), 0);
            recv(client_fd, &b, sizeof(b), 0);

            int sum = a + b;
            int diff = a - b;

            send(client_fd, &sum, sizeof(sum), 0);
            send(client_fd, &diff, sizeof(diff), 0);
        }

        else if (choice == 2)
        {
            float A, B, C;
            recv(client_fd, &A, sizeof(A), 0);
            recv(client_fd, &B, sizeof(B), 0);
            recv(client_fd, &C, sizeof(C), 0);

            float x = (C - B) / A;

            send(client_fd, &x, sizeof(x), 0);
        }

        else if (choice == 3)
        {
            int r1, c1, r2, c2;
            recv(client_fd, &r1, sizeof(r1), 0);
            recv(client_fd, &c1, sizeof(c1), 0);
            recv(client_fd, &r2, sizeof(r2), 0);
            recv(client_fd, &c2, sizeof(c2), 0);

            int mat1[MAX][MAX], mat2[MAX][MAX], result[MAX][MAX];

            recv(client_fd, mat1, sizeof(mat1), 0);
            recv(client_fd, mat2, sizeof(mat2), 0);

            memset(result, 0, sizeof(result));

            for (int i = 0; i < r1; i++)
            {
                for (int j = 0; j < c2; j++)
                {
                    for (int k = 0; k < c1; k++)
                    {
                        result[i][j] += mat1[i][k] * mat2[k][j];
                    }
                }
            }

            send(client_fd, result, sizeof(result), 0);
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
