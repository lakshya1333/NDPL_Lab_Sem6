// Create a Book database at the server side and store the following information: title,
// author, accession number, total pages, and the publisher. Write C programs to
// implement the following client-server model:
// a. Insert new book information
// b. Delete a book
// c. Display all book information
// d. Search a book (Based on Title or author)
// e. Exit
// At the client side, the user selects the required option and sends it along with the
// necessary information to the server and server will perform the requested operation.
// Server should send appropriate messages back to the client informing the success or
// failure of the requested operation. Client should continue to request the operation
// until user selects the option ”Exit”. To search the book by author name, the client
// program should send the name of the author to the server. The list of all book details
// for that author should be sent to the client. If the author name is not found, then
// server should send appropriate message to the client.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BOOKS 100
#define BUF_SIZE 2048

/* -------- Book Structure -------- */
struct Book
{
    char title[100];
    char author[100];
    int accession;
    int pages;
    char publisher[100];
};

struct Book db[MAX_BOOKS];
int bookCount = 0;

/* -------- Insert Book -------- */
void insertBook(struct Book b)
{
    if (bookCount < MAX_BOOKS)
    {
        db[bookCount++] = b;
    }
}

/* -------- Delete Book by Accession -------- */
int deleteBook(int acc)
{
    for (int i = 0; i < bookCount; i++)
    {
        if (db[i].accession == acc)
        {
            // Shift remaining books
            for (int j = i; j < bookCount - 1; j++)
                db[j] = db[j + 1];

            bookCount--;
            return 1; // Deleted successfully
        }
    }
    return 0; // Not found
}

/* -------- Display All Books -------- */
void displayBooks(char *result)
{
    if (bookCount == 0)
    {
        strcpy(result, "No books available in database.\n");
        return;
    }

    strcpy(result, "\n--- Book Database ---\n");

    for (int i = 0; i < bookCount; i++)
    {
        char temp[300];
        sprintf(temp,
                "\nTitle: %s\nAuthor: %s\nAccession: %d\nPages: %d\nPublisher: %s\n",
                db[i].title, db[i].author, db[i].accession,
                db[i].pages, db[i].publisher);

        strcat(result, temp);
    }
}

/* -------- Search by Title -------- */
int searchByTitle(char *title, char *result)
{
    for (int i = 0; i < bookCount; i++)
    {
        if (strcasecmp(db[i].title, title) == 0)
        {
            sprintf(result,
                    "Book Found!\nTitle: %s\nAuthor: %s\nAccession: %d\nPages: %d\nPublisher: %s\n",
                    db[i].title, db[i].author, db[i].accession,
                    db[i].pages, db[i].publisher);
            return 1;
        }
    }
    return 0;
}

/* -------- Search by Author -------- */
int searchByAuthor(char *author, char *result)
{
    int found = 0;
    strcpy(result, "\nBooks by Author:\n");

    for (int i = 0; i < bookCount; i++)
    {
        if (strcasecmp(db[i].author, author) == 0)
        {
            char temp[300];
            sprintf(temp,
                    "\nTitle: %s | Accession: %d | Pages: %d | Publisher: %s\n",
                    db[i].title, db[i].accession,
                    db[i].pages, db[i].publisher);

            strcat(result, temp);
            found = 1;
        }
    }
    return found;
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    int choice;
    char buffer[BUF_SIZE];

    /* ---- Create TCP Socket ---- */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("✅ Book Database Server running on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);

    while (1)
    {
        recv(client_fd, &choice, sizeof(choice), 0);

        if (choice == 5)
            break;

        memset(buffer, 0, BUF_SIZE);

        /* ================= INSERT ================= */
        if (choice == 1)
        {
            struct Book b;
            recv(client_fd, &b, sizeof(b), 0);

            insertBook(b);
            strcpy(buffer, "Book inserted successfully!");
            send(client_fd, buffer, BUF_SIZE, 0);
        }

        /* ================= DELETE ================= */
        else if (choice == 2)
        {
            int acc;
            recv(client_fd, &acc, sizeof(acc), 0);

            if (deleteBook(acc))
                strcpy(buffer, "Book deleted successfully!");
            else
                strcpy(buffer, "Book not found!");

            send(client_fd, buffer, BUF_SIZE, 0);
        }

        /* ================= DISPLAY ================= */
        else if (choice == 3)
        {
            displayBooks(buffer);
            send(client_fd, buffer, BUF_SIZE, 0);
        }

        /* ================= SEARCH ================= */
        else if (choice == 4)
        {
            int type;
            recv(client_fd, &type, sizeof(type), 0);

            char query[100];
            recv(client_fd, query, sizeof(query), 0);

            if (type == 1)
            {
                if (!searchByTitle(query, buffer))
                    strcpy(buffer, "Book not found by title!");
            }
            else if (type == 2)
            {
                if (!searchByAuthor(query, buffer))
                    strcpy(buffer, "No books found for this author!");
            }

            send(client_fd, buffer, BUF_SIZE, 0);
        }
    }

    close(client_fd);
    close(server_fd);

    return 0;
}