#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

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

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[300];

    char data[200];
    unsigned int received_crc;
    int choice;

    unsigned int poly, calculated_crc;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd,(struct sockaddr*)&address,sizeof(address));

    listen(server_fd,3);

    printf("Receiver waiting for sender...\n");

    new_socket = accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen);

    read(new_socket,buffer,300);

    sscanf(buffer,"%[^|]|%u|%d",data,&received_crc,&choice);

    if(choice==1) poly = 0x80F;
    else if(choice==2) poly = 0x8005;
    else poly = 0x1021;

    calculated_crc = compute_crc(data,poly);

    printf("\nReceived data: %s\n",data);
    printf("Received CRC: %u\n",received_crc);
    printf("Calculated CRC: %u\n",calculated_crc);

    if(received_crc == calculated_crc)
        printf("\nData NOT corrupted\n");
    else
        printf("\nData CORRUPTED\n");

    close(new_socket);
    close(server_fd);

    return 0;
}