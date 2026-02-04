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
    {"101", "Alice", "123 Maple St", "CSE", 4, 'A', "OS, CN, DBMS", "CS301", 85},
    {"102", "Bob", "456 Oak Ave", "ECE", 6, 'B', "VLSI, DSP", "EC402", 78}
};

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    struct Request req;
    char result[1024];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8081);

    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("UDP Server listening on port 8081...\n");

    while (1) {
        if (recvfrom(sock, &req, sizeof(req), 0, (struct sockaddr*)&client_addr, &addr_len) > 0) {
            
            if (fork() == 0) { 
                int found = 0;
                memset(result, 0, sizeof(result));

                for (int i = 0; i < 2; i++) {
                    if ((req.choice == 1 && strcmp(db[i].reg_no, req.detail) == 0) ||
                        (req.choice == 2 && strcmp(db[i].name, req.detail) == 0) ||
                        (req.choice == 3 && strcmp(db[i].sub_code, req.detail) == 0)) {
                        
                        found = 1;
                        if (req.choice == 1)
                            sprintf(result, "PID: %d | Name: %s, Addr: %s", getpid(), db[i].name, db[i].address);
                        else if (req.choice == 2)
                            sprintf(result, "PID: %d | Dept: %s, Sem: %d, Sec: %c", getpid(), db[i].dept, db[i].semester, db[i].section);
                        else if (req.choice == 3)
                            sprintf(result, "PID: %d | Marks: %d", getpid(), db[i].marks);
                        break;
                    }
                }
                if (!found) sprintf(result, "PID: %d | Not Found.", getpid());

                sendto(sock, result, strlen(result), 0, (struct sockaddr*)&client_addr, addr_len);
                exit(0);
            }
            waitpid(-1, NULL, WNOHANG);
        }
    }
    return 0;
}