/**********************************************
 *  file: client.c 客户端代码
 *  功能： 完成客户端的接口
 *  auth: kmist
 * *******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    int serverfd = -1;
    struct sockaddr_in server;

    char recvbuf[BUFSIZE] = {0};
    char sendbuf[BUFSIZE] = {0};

    if(argc < 3)
    {
        fprintf(stderr, "usarg <ip> <port>");
        exit(1);
    }
    /* 1. socket */
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd < 0)
    {
        perror("socket()");
        exit(1);
    }

    /* 2. connect */
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(serverfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect()");
        close(serverfd);
        exit(1);
    }

    sprintf(sendbuf, "%s", "hello,server");
    send(serverfd, sendbuf,strlen(sendbuf),0);

    memset(recvbuf, 0, sizeof(recvbuf));
    memset(sendbuf, 0, sizeof(sendbuf));
    while(1)
    {   
        printf("send:\n");
        fgets(sendbuf, sizeof(sendbuf), stdin);
        sendbuf[strlen(sendbuf)-1] = '\0';
        send(serverfd, sendbuf, strlen(sendbuf), 0);

        recv(serverfd, recvbuf, sizeof(recvbuf), 0);
        printf("recv[%s]\n",recvbuf);

        memset(recvbuf, 0, sizeof(recvbuf));
        memset(sendbuf, 0, sizeof(sendbuf));
    }
      

    close(serverfd);

}
