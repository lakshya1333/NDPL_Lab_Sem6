#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 9090
#define BUF_SIZE 1024

struct Student
{
    char regNo[20];
    char name[50];
    char address[100];

    char dept[20];
    int semester;
    char section[5];
    char courses[100];

    char subjectCode[10];
    int marks;
};


struct Student db[] =
{
    {"21CSE101", "Lakshya", "Delhi, India",
     "CSE", 4, "A", "CN, OS, DBMS",
     "CS301", 87},

    {"21CSE102", "Shinchan", "Tokyo, Japan",
     "CSE", 3, "B", "DSA, OOP, COA",
     "CS302", 91}
};

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);

    int option;
    char input[BUF_SIZE], output[BUF_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("✅ UDP Student Server running on port %d...\n", PORT);

    while (1)
    {
        recvfrom(sockfd, &option, sizeof(option), 0,
                 (struct sockaddr *)&client_addr, &len);

        if (option == 4)
        {
            printf("Client exited.\n");
            break;
        }

        memset(input, 0, BUF_SIZE);
        recvfrom(sockfd, input, BUF_SIZE, 0,
                 (struct sockaddr *)&client_addr, &len);

        pid_t pid = fork();

        if (pid == 0)
        {

            memset(output, 0, BUF_SIZE);

            if (option == 1)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (strcmp(db[i].regNo, input) == 0)
                    {
                        sprintf(output,
                                "Child-1 PID=%d\nRegNo=%s\nName=%s\nAddress=%s\n",
                                getpid(), db[i].regNo,
                                db[i].name, db[i].address);
                        break;
                    }
                }
            }

            else if (option == 2)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (strcmp(db[i].name, input) == 0)
                    {
                        sprintf(output,
                                "Child-2 PID=%d\nName=%s\nDept=%s\nSemester=%d\nSection=%s\nCourses=%s\n",
                                getpid(), db[i].name,
                                db[i].dept, db[i].semester,
                                db[i].section, db[i].courses);
                        break;
                    }
                }
            }

            else if (option == 3)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (strcmp(db[i].subjectCode, input) == 0)
                    {
                        sprintf(output,
                                "Child-3 PID=%d\nSubject=%s\nMarks=%d\n",
                                getpid(), db[i].subjectCode,
                                db[i].marks);
                        break;
                    }
                }
            }

            if (strlen(output) == 0)
                sprintf(output, "PID=%d\nRecord Not Found!\n", getpid());

            sendto(sockfd, output, BUF_SIZE, 0,
                   (struct sockaddr *)&client_addr, len);

            exit(0);
        }

        /* ---- Parent waits ---- */
        wait(NULL);
    }

    return 0;
}