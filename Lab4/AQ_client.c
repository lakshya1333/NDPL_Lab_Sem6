#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 2048

struct Book
{
    char title[100];
    char author[100];
    int accession;
    int pages;
    char publisher[100];
};

int main()
{
    int sock;
    struct sockaddr_in serv_addr;

    int choice;
    char buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1)
    {
        printf("\n====== BOOK DATABASE MENU ======\n");
        printf("1. Insert new book\n");
        printf("2. Delete a book\n");
        printf("3. Display all books\n");
        printf("4. Search a book\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        send(sock, &choice, sizeof(choice), 0);

        if (choice == 5)
            break;

        /* ================= INSERT ================= */
        if (choice == 1)
        {
            struct Book b;

            printf("Enter Title: ");
            scanf(" %[^\n]", b.title);

            printf("Enter Author: ");
            scanf(" %[^\n]", b.author);

            printf("Enter Accession Number: ");
            scanf("%d", &b.accession);

            printf("Enter Total Pages: ");
            scanf("%d", &b.pages);

            printf("Enter Publisher: ");
            scanf(" %[^\n]", b.publisher);

            send(sock, &b, sizeof(b), 0);
        }

        /* ================= DELETE ================= */
        else if (choice == 2)
        {
            int acc;
            printf("Enter Accession Number to delete: ");
            scanf("%d", &acc);

            send(sock, &acc, sizeof(acc), 0);
        }

        /* ================= SEARCH ================= */
        else if (choice == 4)
        {
            int type;
            char query[100];

            printf("Search By:\n1. Title\n2. Author\nEnter type: ");
            scanf("%d", &type);

            printf("Enter search text: ");
            scanf(" %[^\n]", query);

            send(sock, &type, sizeof(type), 0);
            send(sock, query, sizeof(query), 0);
        }

        /* ---- Receive Response ---- */
        memset(buffer, 0, BUF_SIZE);
        recv(sock, buffer, BUF_SIZE, 0);

        printf("\n--- Server Response ---\n%s\n", buffer);
    }

    close(sock);
    return 0;
}