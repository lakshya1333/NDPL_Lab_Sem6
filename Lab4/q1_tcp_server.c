#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

struct Student {
    char reg_no[20];
    char name[50];
    char address[100];
    char dept[20];
    int semester;
    char section;
    char courses[100];
    char sub_code[20];
    int marks;
};

struct Request {
    int choice;
    char detail[50];
};


struct Student db[] = {
    {"230953334", "Lakshya", "Udupi", "CCE", 5, 'C', "OS, CN, DBMS", "CS301", 85},
    {"230953338", "ADITYA", "Udupi", "CCE", 5, 'C', "OS, CN, DBMS", "CS301", 85}
};

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct Request req;
    char result[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(4040);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Server listening on port 8080...\n");

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        
        if (recv(new_socket, &req, sizeof(req), 0) > 0) {
            pid_t pid = fork();

            if (pid == 0) { 
                int found = 0;
                memset(result, 0, sizeof(result));

                for (int i = 0; i < 2; i++) {
                    if ((req.choice == 1 && strcmp(db[i].reg_no, req.detail) == 0) ||
                        (req.choice == 2 && strcmp(db[i].name, req.detail) == 0) ||
                        (req.choice == 3 && strcmp(db[i].sub_code, req.detail) == 0)) {
                        
                        found = 1;
                        if (req.choice == 1) {
                            sprintf(result, "Child PID: %d\nName: %s\nAddress: %s", 
                                    getpid(), db[i].name, db[i].address);
                        } else if (req.choice == 2) {
                            sprintf(result, "Child PID: %d\nDept: %s\nSem: %d\nSec: %c\nCourses: %s", 
                                    getpid(), db[i].dept, db[i].semester, db[i].section, db[i].courses);
                        } else if (req.choice == 3) {
                            sprintf(result, "Child PID: %d\nMarks in %s: %d", 
                                    getpid(), db[i].sub_code, db[i].marks);
                        }
                        break;
                    }
                }

                if (!found) sprintf(result, "Child PID: %d\nRecord not found.", getpid());
                
                send(new_socket, result, strlen(result), 0);
                close(new_socket);
                exit(0); 
            }
            close(new_socket);
            waitpid(-1, NULL, WNOHANG); 
        }
    }
    return 0;
}