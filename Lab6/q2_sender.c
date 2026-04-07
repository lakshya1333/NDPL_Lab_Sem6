#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

unsigned int compute_crc(char *data, unsigned int poly) {
    unsigned int crc = 0;
    int i;

    while (*data) {
        crc ^= (*data << 8);

        for (i = 0; i < 8; i++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ poly;
            else
                crc <<= 1;
        }

        data++;
    }

    return crc & 0xFFFF;
}

int main() {

    int sock;
    struct sockaddr_in serv_addr;

    char data[200];
    char message[300];

    int choice;
    unsigned int crc, poly;

    printf("Enter data to send: ");
    scanf("%s", data);

    printf("\nSelect CRC Technique\n");
    printf("1. CRC-12\n");
    printf("2. CRC-16\n");
    printf("3. CRC-CCITT\n");
    scanf("%d", &choice);

    if(choice==1) poly = 0x80F;
    else if(choice==2) poly = 0x8005;
    else poly = 0x1021;

    crc = compute_crc(data, poly);

    sprintf(message,"%s|%u|%d",data,crc,choice);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr);

    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    send(sock,message,strlen(message),0);

    printf("\nData sent: %s\n",data);
    printf("CRC value: %u\n",crc);

    close(sock);

    return 0;
}