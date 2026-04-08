#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<ctype.h>

#define MAXSIZE 500

main()
{
    int sockfd, newsockfd, retval, i, j;
    socklen_t actuallen;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE], filename[MAXSIZE], str1[MAXSIZE], str2[MAXSIZE], result[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 1);
    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &actuallen);

    memset(filename, '\0', sizeof(filename));
    recv(newsockfd, filename, sizeof(filename), 0);

    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
    {
        send(newsockfd, "File not present", 16, 0);
        close(newsockfd);
        close(sockfd);
        return;
    }
    fclose(fp);
    send(newsockfd, "File found", 10, 0);

    while(1)
    {
        memset(buff, '\0', sizeof(buff));
        if(recv(newsockfd, buff, sizeof(buff), 0) <= 0) break;
        int choice = atoi(buff);
        if(choice == 4) break;

        if(choice == 1)
        {
            memset(str1, '\0', sizeof(str1));
            recv(newsockfd, str1, sizeof(str1), 0);
            fp = fopen(filename, "r");
            int count = 0;
            while(fscanf(fp, "%s", buff) != EOF)
            {
                int len = strlen(buff);
                while(len > 0 && ispunct(buff[len-1])) {
                    buff[len-1] = '\0';
                    len--;
                }
                if(strcmp(buff, str1) == 0) count++;
            }
            fclose(fp);
            if(count > 0) sprintf(result, "Occurrences: %d", count);
            else strcpy(result, "String not found");
        }
        else if(choice == 2)
        {
            memset(str1, '\0', sizeof(str1));
            recv(newsockfd, str1, sizeof(str1), 0);
            memset(str2, '\0', sizeof(str2));
            recv(newsockfd, str2, sizeof(str2), 0);

            fp = fopen(filename, "r");
            FILE *ftemp = fopen("temp.txt", "w");
            int found = 0;
            while(fscanf(fp, "%s", buff) != EOF)
            {
                char punctuation[10] = "";
                int len = strlen(buff);
                int p_idx = 0;
                while(len > 0 && ispunct(buff[len-1])) {
                    punctuation[p_idx++] = buff[len-1];
                    buff[--len] = '\0';
                }

                if(strcmp(buff, str1) == 0) {
                    fprintf(ftemp, "%s", str2);
                    found = 1;
                }
                else fprintf(ftemp, "%s", buff);

                for(int k = p_idx-1; k >= 0; k--) fputc(punctuation[k], ftemp);
                fputc(' ', ftemp);
            }
            fclose(fp);
            fclose(ftemp);
            remove(filename);
            rename("temp.txt", filename);
            if(found) strcpy(result, "String replaced");
            else strcpy(result, "String not found");
        }
        else if(choice == 3)
        {
            char words[100][MAXSIZE];
            int word_count = 0;
            fp = fopen(filename, "r");
            while(fscanf(fp, "%s", words[word_count]) != EOF) {
                word_count++;
            }
            fclose(fp);

            for(i=0; i < word_count-1; i++) {
                for(j=0; j < word_count-i-1; j++) {
                    if(strcmp(words[j], words[j+1]) > 0) {
                        char temp[MAXSIZE];
                        strcpy(temp, words[j]);
                        strcpy(words[j], words[j+1]);
                        strcpy(words[j+1], temp);
                    }
                }
            }

            fp = fopen(filename, "w");
            for(i=0; i < word_count; i++) {
                fprintf(fp, "%s ", words[i]);
            }
            fclose(fp);
            strcpy(result, "File reordered by words");
        }
        send(newsockfd, result, strlen(result), 0);
    }
    close(newsockfd);
    close(sockfd);
}
