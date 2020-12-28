/**********************************************
 *  file: server.c 服务端代码
 *  功能： 完成服务端的接口
 *  auth: kmist
 * *******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <time.h>

#include "mytbf.h"

#define BUFSIZE     1204
#define CPS         5
#define BURST       100

static int startup(unsigned short port)
{
    int sock = -1;
    struct sockaddr_in server;

    /* 1. socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /* 2. bind */
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind()");
        close(sock);
        exit(1);
    }

    /* 3. listen */
    listen(sock, 5); //同时最多监听5个客户端

    return sock;
}


static int find_free(int *array, size_t len)
{
    int i = 0;
    int pos = -1;
    while(i < len)
    {
        if(array[i] == -1)
        {
              pos = i;
              break;
        }
          
        i++;
    }
    return pos;
}

// ./server 8888
int main(int argc, char *argv[])
{
    int listenfd = -1;
    int clientfd[10] = {-1};
    struct sockaddr_in client;
    socklen_t clilen;
    pid_t pid;
    int cur = 0;

    if(argc < 2)
    {
        fprintf(stderr, "usarg <port>");
        exit(1);
    }

    listenfd =  startup(atoi(argv[1]));
    clilen = sizeof(client);
    while(1)
    {
        clientfd[cur] = accept(listenfd, (struct sockaddr *)&client, &clilen); //客户端的结构
        
        if(cur > 5)
        {
            cur = 5;
        }
        /* 多进程 pid <0; =0; >0 */
        pid = fork();
        if(pid < 0)
        {
            perror("fork()");
        }
        if(pid == 0) //子进程
        {
            close(listenfd); //zi会拷贝父进程所有资源，包括listenfd,所以这里将它关闭
            char recvbuf[BUFSIZE] = {0};
            char sendbuf[BUFSIZE] = {0};
            int to = 0;
            mytbf_t *mytbf;
            mytbf = mytbf_init(CPS, BURST); //初始化令牌桶

            while(1)
            {
                to = mytbf_gettoken(mytbf, 5);      //一次消耗5个token，可随意设置
                if(to == 5) //如果拿到了5个token
                {
                    if(recv(clientfd[cur], recvbuf, sizeof(recvbuf), 0) <= 0)
                    {
                        break;
                    }
                    printf("client[%d]-> recv[%s],left token:[%d]\n",cur, recvbuf,mytbf_lefttoken(mytbf));

                    if(!strcmp(recvbuf, "hello"))
                    {
                        sprintf(sendbuf, "%s", "hello, client");
                    
                    }
                    if(!strcmp(recvbuf, "time"))
                    {
                        sprintf(sendbuf, "%ld", time(NULL));
                    
                    }
                    if(!strcmp(recvbuf, "quit"))
                    {
                        break;
                    }
                    send(clientfd[cur],sendbuf, strlen(sendbuf), 0);
                    memset(recvbuf, 0, sizeof(recvbuf));
                    memset(sendbuf, 0, sizeof(sendbuf));
                }
               
            }
            close(clientfd[cur]);
            mytbf_destory(mytbf);
            exit(0); //！非常重要
        }

       
        
        //父进程
        cur++;
    }   
wait(NULL); //等待子进程结束收尸
}


