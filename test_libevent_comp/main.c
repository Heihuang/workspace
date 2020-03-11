#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include "event2/event.h"

struct event_base *base;
int PORT = 9999;
int BACKLOG = 5;

int create_bind_listen()
{
    struct sockaddr_in my_addr;
    int yes = 1;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    memset(&my_addr, 0, sizeof(struct sockaddr_in));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));
    listen(sock, BACKLOG);
    return sock;
}

void cb_func(evutil_socket_t fd, short what, void *arg)
{
    printf("%d\n", fd);
}

int main()
{
    /*int sock = create_bind_listen();
    struct event *ev;
    base = event_base_new();
       
    ev = event_new(base, sock,
                   (short)EV_READ|EV_PERSIST,
                   cb_func, NULL);
    event_add(ev, NULL);
    event_base_dispatch(base);*/
    event_init();
    return 0;
}
