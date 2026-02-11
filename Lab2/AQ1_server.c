// Write two separate C programs (one for server and other for client) using socket APIs
// for TCP and UDP, to implement the File Server. The client program will send the
// name of the text file to the server. If the file is present at the server side, the server
// should send the contents of the file to the client along with the file size, number of
// alphabets number of lines, number of spaces, number of digits, and number of other
// characters present in the text file to the client. If the file is not present, then the
// server should send the proper message to the client. Note that the results are always
// displayed at the client side. Client should continue to send the filenames until the
// user enters the string ‘stop’.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

void file_stats(FILE *fp, int *alph, int *dig, int *sp, int *lines, int *other, long *size)
{
    char ch;
    *alph = *dig = *sp = *lines = *other = 0;
    *size = 0;

    while ((ch = fgetc(fp)) != EOF)
    {
        (*size)++;

        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
            (*alph)++;
        else if (ch >= '0' && ch <= '9')
            (*dig)++;
        else if (ch == ' ')
            (*sp)++;
        else if (ch == '\n')
            (*lines)++;
        else
            (*other)++;
    }
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    char filename[BUF_SIZE], buffer[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));

    listen(server_fd, 5);
    printf("✅ TCP File Server running on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);

    while (1)
    {
        memset(filename, 0, BUF_SIZE);

        recv(client_fd, filename, BUF_SIZE, 0);

        if (strcmp(filename, "stop") == 0)
            break;

        FILE *fp = fopen(filename, "r");

        if (!fp)
        {
            strcpy(buffer, "File not found!");
            send(client_fd, buffer, BUF_SIZE, 0);
            continue;
        }

        // File exists
        strcpy(buffer, "File found!");
        send(client_fd, buffer, BUF_SIZE, 0);

        int alph, dig, sp, lines, other;
        long size;

        file_stats(fp, &alph, &dig, &sp, &lines, &other, &size);

        rewind(fp); // Go back to beginning

        // Send statistics
        sprintf(buffer,
                "File Size: %ld bytes\nAlphabets: %d\nDigits: %d\nSpaces: %d\nLines: %d\nOther Characters: %d\n",
                size, alph, dig, sp, lines, other);

        send(client_fd, buffer, BUF_SIZE, 0);

        // Send file content
        while (fgets(buffer, BUF_SIZE, fp))
        {
            send(client_fd, buffer, BUF_SIZE, 0);
        }

        // End marker
        strcpy(buffer, "EOF");
        send(client_fd, buffer, BUF_SIZE, 0);

        fclose(fp);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}