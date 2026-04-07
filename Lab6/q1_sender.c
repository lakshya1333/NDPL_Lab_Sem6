#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

int main() {

    int sock;
    struct sockaddr_in serv_addr;
    char data[100], sendData[100];
    int i, count = 0, parityType;
    char parityBit;

    printf("Enter binary data: ");
    scanf("%s", data);

    printf("Choose parity type:\n1. Even\n2. Odd\n");
    scanf("%d", &parityType);

    for(i = 0; i < strlen(data); i++) {
        if(data[i] == '1')
            count++;
    }

    if(parityType == 1) {
        if(count % 2 == 0)
            parityBit = '0';
        else
            parityBit = '1';
    }
    else {
        if(count % 2 == 0)
            parityBit = '1';
        else
            parityBit = '0';
    }

    strcpy(sendData, data);
    int len = strlen(sendData);

    sendData[len] = parityBit;
    sendData[len+1] = '\0';

    printf("Transmitting data: %s\n", sendData);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    send(sock, sendData, strlen(sendData), 0);

    printf("Data sent successfully\n");

    close(sock);

    return 0;
}