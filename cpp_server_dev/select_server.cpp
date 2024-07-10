#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>
#include<string.h>
#include<sys/time.h>
#include<vector>
#include<errno.h>

#define INVALID_FD -1

int main(int argc, char* argv[]){
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == INVALID_FD){
        std::cout<<"create listen socket failed"<<std::endl;
        return -1;
    }

    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if(bind(listenfd, (struct sockaddr*)&bindaddr, sizeof(bindaddr)) == -1){
        std::cout<<"bind error"<<std::endl;
        close(listenfd);
        return -1;
    }

    if(listen(listenfd, SOMAXCONN) == -1){
        std::cout<<"listen failed"<<std::endl;
        close(listenfd);
        return -1;
    }

    std::vector<int> clientfds;
    int maxfd;

    while(true){
        fd_set readset;
        FD_ZERO(&readset);

        FD_SET(listenfd, &readset);

        maxfd = listenfd;

        int clientfdslength = clientfds.size();
        for(int i = 0;i<clientfdslength;++i){
            if(clientfds[i] != INVALID_FD){
                FD_SET(clientfds[i], &readset);

                if(maxfd< clientfds[i]) maxfd = clientfds[i];
            }
        }

        timeval tm;
        tm.tv_sec = 1;
        tm.tv_usec = 0;

        int ret = select(maxfd+1, &readset, NULL, NULL, &tm);
        if(ret == -1){
            if(errno != EINTR) break;
            }
        else if(ret == 0) continue;
        else{
            if(FD_ISSET(listenfd, &readset)){ //有连接请求
                struct sockaddr_in clientaddr;
                socklen_t clientaddrlen = sizeof(clientaddr);
                int clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
                if(clientfd == INVALID_FD) break;
                std::cout<<"accept successfully, fd: "<<clientfd<<std::endl;
                clientfds.push_back(clientfd);
            }
            else{ //检测监听的客户端fd有无消息
                char recvBuf[64];
                int clientfdslength = clientfds.size();
                for(int i=0;i<clientfdslength;++i){
                    if(clientfds[i]!=INVALID_FD&&FD_ISSET(clientfds[i], &readset)){
                        memset(recvBuf, 0, sizeof(recvBuf));
                        int length = recv(clientfds[i], recvBuf, 64, 0);
                        if(length<=0){
                            std::cout<<"recv data error, clientfd: "<<clientfds[i]<<std::endl;
                            close(clientfds[i]);
                            clientfds[i] = INVALID_FD;
                            continue;
                        }
                        std::cout<<"clientfd: "<<clientfds[i] <<", recv data: "<<recvBuf<<std::endl;
                    }
                }
            }

        }
    }
    int clientfdslength = clientfds.size();
    for(int i = 0;i<clientfdslength;++i){
        if(clientfds[i] != INVALID_FD) close(clientfds[i]);
    }
    close(listenfd);
    return 0;
}