#include <stdio.h>
#include <math.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {

    int server_fd,new_socket;
    struct sockaddr_in address;
    int addrlen=sizeof(address);

    char buffer[200];
    int code[30];

    int i,j,k,r=0,total,error=0;

    server_fd = socket(AF_INET,SOCK_STREAM,0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd,(struct sockaddr*)&address,sizeof(address));

    listen(server_fd,3);

    printf("Receiver waiting...\n");

    new_socket = accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen);

    read(new_socket,buffer,200);

    total = strlen(buffer);

    for(i=1;i<=total;i++)
        code[i] = buffer[i-1]-'0';

    while(pow(2,r) < total+1)
        r++;

    for(i=0;i<r;i++){
        int p=pow(2,i);
        int count=0;

        for(j=p;j<=total;j+=2*p){
            for(k=j;k<j+p && k<=total;k++){
                if(code[k]==1)
                    count++;
            }
        }

        if(count%2!=0)
            error += p;
    }

    printf("\nReceived Code:\n");
    for(i=1;i<=total;i++)
        printf("%d ",code[i]);

    if(error==0){
        printf("\n\nNo error detected\n");
    }
    else{
        printf("\n\nError detected at position %d\n",error);

        code[error] = !code[error];

        printf("Corrected Code:\n");

        for(i=1;i<=total;i++)
            printf("%d ",code[i]);
    }

    printf("\n");

    close(new_socket);
    close(server_fd);

    return 0;
}