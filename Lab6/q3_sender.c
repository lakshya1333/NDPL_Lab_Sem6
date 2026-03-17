#include <stdio.h>
#include <math.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {

    int data[20], code[30];
    int i,j,k,n,r=0,pos=0,total;
    int sock;
    struct sockaddr_in serv_addr;
    char message[200];

    printf("Enter number of data bits: ");
    scanf("%d",&n);

    printf("Enter data bits:\n");
    for(i=0;i<n;i++)
        scanf("%d",&data[i]);

    while(pow(2,r) < (n+r+1))
        r++;

    total = n + r;

    j=0;
    for(i=1;i<=total;i++){
        if(i==pow(2,j)){
            code[i]=0;
            j++;
        }
        else{
            code[i]=data[pos++];
        }
    }

    for(i=0;i<r;i++){
        int p=pow(2,i);
        int count=0;

        for(j=p;j<=total;j+=2*p){
            for(k=j;k<j+p && k<=total;k++){
                if(code[k]==1)
                    count++;
            }
        }

        code[p]=count%2;
    }

    printf("\nHamming Code Sent:\n");

    message[0]='\0';

    for(i=1;i<=total;i++){
        printf("%d ",code[i]);
        char temp[5];
        sprintf(temp,"%d",code[i]);
        strcat(message,temp);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr);

    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    send(sock,message,strlen(message),0);

    printf("\nData transmitted to receiver\n");

    close(sock);

    return 0;
}