
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 10

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    int choice;

    /* -------- Create Socket -------- */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    /* -------- Connect -------- */
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("✅ Connected to Calculator Server!\n");

    while (1)
    {
        printf("\n========= MENU =========\n");
        printf("1. Add/Subtract two integers\n");
        printf("2. Solve linear equation (ax + b = c)\n");
        printf("3. Multiply two matrices\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        send(sock, &choice, sizeof(choice), 0);

        if (choice == 4)
        {
            printf("Exiting...\n");
            break;
        }

        /* ================= OPTION 1 ================= */
        if (choice == 1)
        {
            int a, b;
            printf("Enter two integers: ");
            scanf("%d %d", &a, &b);

            send(sock, &a, sizeof(a), 0);
            send(sock, &b, sizeof(b), 0);

            int sum, diff;
            recv(sock, &sum, sizeof(sum), 0);
            recv(sock, &diff, sizeof(diff), 0);

            printf("Addition = %d\n", sum);
            printf("Subtraction = %d\n", diff);
        }

        /* ================= OPTION 2 ================= */
        else if (choice == 2)
        {
            float A, B, C;
            printf("Equation: ax + b = c\n");
            printf("Enter a, b, c: ");
            scanf("%f %f %f", &A, &B, &C);

            send(sock, &A, sizeof(A), 0);
            send(sock, &B, sizeof(B), 0);
            send(sock, &C, sizeof(C), 0);

            float x;
            recv(sock, &x, sizeof(x), 0);

            printf("Value of x = %.2f\n", x);
        }

        /* ================= OPTION 3 ================= */
        else if (choice == 3)
        {
            int r1, c1, r2, c2;
            int mat1[MAX][MAX], mat2[MAX][MAX], result[MAX][MAX];

            printf("Enter rows and cols of Matrix 1: ");
            scanf("%d %d", &r1, &c1);

            printf("Enter rows and cols of Matrix 2: ");
            scanf("%d %d", &r2, &c2);

            if (c1 != r2)
            {
                printf("Matrix multiplication not possible!\n");
                continue;
            }

            printf("Enter elements of Matrix 1:\n");
            for (int i = 0; i < r1; i++)
                for (int j = 0; j < c1; j++)
                    scanf("%d", &mat1[i][j]);

            printf("Enter elements of Matrix 2:\n");
            for (int i = 0; i < r2; i++)
                for (int j = 0; j < c2; j++)
                    scanf("%d", &mat2[i][j]);

            send(sock, &r1, sizeof(r1), 0);
            send(sock, &c1, sizeof(c1), 0);
            send(sock, &r2, sizeof(r2), 0);
            send(sock, &c2, sizeof(c2), 0);

            send(sock, mat1, sizeof(mat1), 0);
            send(sock, mat2, sizeof(mat2), 0);

            recv(sock, result, sizeof(result), 0);

            printf("\nResult Matrix:\n");
            for (int i = 0; i < r1; i++)
            {
                for (int j = 0; j < c2; j++)
                    printf("%d ", result[i][j]);
                printf("\n");
            }
        }
    }

    close(sock);
    return 0;
}