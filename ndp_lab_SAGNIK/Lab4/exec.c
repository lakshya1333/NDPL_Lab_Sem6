#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        printf("Child process (PID %d) is calling exec...\n", getpid());
        
        if (execl("./add", "./add", NULL) == -1) {
            perror("Exec failed");
            exit(1);
        }
    } 
    else {
        printf("Parent (PID %d) is waiting for child...\n", getpid());
        wait(NULL); 
        printf("Child finished. Parent exiting.\n");
    }

    return 0;
}