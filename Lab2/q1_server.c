#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define BUF_SIZE 1024

// Function to count occurrences of a string in file
int search_string(const char *filename, const char *str) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;
    char buffer[BUF_SIZE];
    int count = 0;
    while (fscanf(fp, "%s", buffer) != EOF) {
        if (strcmp(buffer, str) == 0) count++;
    }
    fclose(fp);

    return count;
}


int replace_string(const char *filename, const char *str1, const char *str2) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(fp);
        return -1;
    }

    char line[BUF_SIZE];
    int replaced = 0;

    while (fgets(line, sizeof(line), fp)) {
        char *pos;
        while ((pos = strstr(line, str1)) != NULL) { //Finds substring inside a line.
            // Replace occurrence
            char newLine[BUF_SIZE];
            int lenBefore = pos - line;
            snprintf(newLine, sizeof(newLine), "%.*s%s%s",
                     lenBefore, line, str2, pos + strlen(str1));
            strcpy(line, newLine);
            replaced = 1;
        }
        fputs(line, temp);
    }

    fclose(fp);
    fclose(temp);

    if (replaced) {
        remove(filename);
        rename("temp.txt", filename);
        return 1;
    } else {
        remove("temp.txt");
        return 0;
    }
}

// Function to reorder file contents (sort characters)
void reorder_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char buffer[BUF_SIZE];
    char content[BUF_SIZE * 10] = "";
    while (fgets(buffer, BUF_SIZE, fp)) {
        strcat(content, buffer);
    }
    fclose(fp);

    // Sort characters
    int len = strlen(content);
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (content[i] > content[j]) {
                char temp = content[i];
                content[i] = content[j];
                content[j] = temp;
            }
        }
    }

    fp = fopen(filename, "w");
    fputs(content, fp);
    fclose(fp);
}

int main() {
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind error");
        exit(1);
    }

    listen(sockfd, 5);
    printf("Server listening on port %d...\n", PORT);

    addr_size = sizeof(clientAddr);
    int newSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &addr_size);

    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int recvLen = recv(newSocket, buffer, BUF_SIZE, 0);
        if (recvLen <= 0) break;

        if (strncmp(buffer, "EXIT", 4) == 0) {
            printf("Client disconnected.\n");
            break;
        }

        char command[BUF_SIZE], arg1[BUF_SIZE], arg2[BUF_SIZE], arg3[BUF_SIZE];
        int args = sscanf(buffer, "%s %s %s %s", command, arg1, arg2, arg3);

        if (strcmp(command, "SEARCH") == 0 && args == 3) {
            int count = search_string(arg1, arg2);
            if (count > 0) {
                sprintf(buffer, "String found %d times", count);
            } else {
                strcpy(buffer, "String not found");
            }
        } else if (strcmp(command, "REPLACE") == 0 && args == 4) {
            int res = replace_string(arg1, arg2, arg3);
            if (res) strcpy(buffer, "String replaced");
            else strcpy(buffer, "String not found");
        } else if (strcmp(command, "REORDER") == 0 && args == 2) {
            reorder_file(arg1);
            strcpy(buffer, "File reordered");
        } else {
            strcpy(buffer, "Invalid option or missing arguments");
        }

        send(newSocket, buffer, strlen(buffer), 0);
    }

    close(newSocket);
    close(sockfd);
    return 0;
}
