#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>
#define PORT 8888
#define QUEUE 20

int main(){
    fd_set rfds;
    timeval tv;
    int retval, maxfd;
    int ss = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(ss, (sockaddr*)&server_sockaddr, sizeof(server_sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    if (listen(ss, QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }
    
    sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    int conn = accept(ss, (sockaddr*)&client_addr, &length);
    if (conn < 0)
    {
        perror("connect");
        exit(1);
    }
    while (1)
    {
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        maxfd = 0;
        FD_SET(conn, &rfds);
        if (maxfd < conn)
        {
            maxfd = conn;
        }
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if (retval == -1)
        {
            printf("select error, exit\n");
            break;
        }
        else if(retval == 0){
            printf("Server no input, and Client no sending message, waiting...\n");
            continue;
        }
        else{
            if (FD_ISSET(conn, &rfds))
            {
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));
                int len = recv(conn, buffer, sizeof(buffer), 0);
                if (strcmp(buffer, "exit\n") == 0)
                {
                    break;
                }
                printf("%s", buffer);
                send(conn, buffer, len, 0);
            }
            if (FD_ISSET(0, &rfds))
            {
                char buf[1024];
                fgets(buf, sizeof(buf), stdin);
                printf("You are sending %s", buf);
                send(conn, buf, sizeof(buf), 0);
            }
            
        }
    }
    close(conn);
    close(ss);
    return 0;
}