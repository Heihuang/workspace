#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
using namespace std;
#define MAXLINE 1024
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

void setnonblocking(int sock)//将套接字设置为非阻塞
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}

int InitConn(int &listenfd)
{
    int portnumber = 10020;
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    setnonblocking(listenfd); //把socket设置为非阻塞方式
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    char *local_addr="127.0.0.1";
    inet_aton(local_addr,&(serveraddr.sin_addr)); 
    serveraddr.sin_port=htons(portnumber);
    int res = bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));
    res = listen(listenfd, LISTENQ);
    return res;
}

int InitEvt(int &epfd, struct epoll_event &ev, struct epoll_event *events, const int &listenfd)
{
    epfd=epoll_create(256); //生成用于处理accept的epoll专用的文件描述符
    ev.data.fd=listenfd; //设置与要处理的事件相关的文件描述符
    ev.events=EPOLLIN|EPOLLET;  //设置要处理的事件类型    
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev); //注册epoll事件
    return 0;
}

int Run(const int &epfd, struct epoll_event &ev, struct epoll_event *events, const int &listenfd)
{
    ssize_t n = 0;
    int sockfd = -1;
    socklen_t clilen;
    struct sockaddr_in clientaddr;
    char line[MAXLINE];
    char header[1];
    for ( ; ; ) {
        int nfds=epoll_wait(epfd, events, 20, 500); //等待epoll事件的发生
        for(int i = 0; i< nfds; ++i) //处理所发生的所有事件
        {
            if(events[i].data.fd == listenfd)//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。
            {
                int connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
                if(connfd<0){
                    perror("connfd<0");
                    exit(1);
                }
                char *str = inet_ntoa(clientaddr.sin_addr);
                cout << "accapt a connection from " << str << endl;
                ev.data.fd=connfd; //设置用于读操作的文件描述符
                ev.events=EPOLLIN|EPOLLET; //设置用于注测的读操作事件
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev); //注册ev
            }
            else if(events[i].events&EPOLLIN)//如果是已经连接的用户，并且收到数据，那么进行读入。
            {
                cout << "EPOLLIN" << endl;
                if ( (sockfd = events[i].data.fd) < 0)
                    continue;
                //n = read(sockfd, header, 1);
                //cout << "head " << header << endl;
                if ( (n = read(sockfd, line, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        close(sockfd);
                        events[i].data.fd = -1;
                    } else
                        std::cout<<"readline error"<<std::endl;
                } else if (n == 0) {
                    close(sockfd);
                    events[i].data.fd = -1;
                }
                line[n] = '\0';
                cout << "read " << line << endl;
		sleep(2);
                //ev.data.fd=sockfd;  //设置用于写操作的文件描述符
                //ev.events=EPOLLOUT|EPOLLET; //设置用于注测的写操作事件
                //epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev); //修改sockfd上要处理的事件为EPOLLOUT
            }
            else if(events[i].events&EPOLLOUT) // 如果有数据发送
            {
                /*sockfd = events[i].data.fd;
                write(sockfd, line, n);
                ev.data.fd=sockfd; //设置用于读操作的文件描述符
                ev.events=EPOLLIN|EPOLLET; //设置用于注测的读操作事件
                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);  //修改sockfd上要处理的事件为EPOLIN
		*/
            }
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    int i, listenfd, sockfd,epfd,nfds;
    struct epoll_event ev,events[20]; //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
    InitConn(listenfd);
    InitEvt(epfd, ev, events, listenfd);
    Run(epfd, ev, events, listenfd);
    return 0;
}
