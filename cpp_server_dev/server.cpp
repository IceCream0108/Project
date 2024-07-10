#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>
#include<string.h>

int main(int argc, char* argv[]){
    //1.创建监听socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1){
        std::cout<<"create listenfd failed"<<std::endl;
        return -1;
    }

    //2.初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if(bind(listenfd, (struct sockaddr*)&bindaddr, sizeof(bindaddr)) == -1){
        std::cout<<"bind listenfd failid"<<std::endl;
        return -1;
    }
    
    //3.启动监听
    if(listen(listenfd, SOMAXCONN) == -1){
        std::cout<<"listen failed"<<std::endl;
        return -1;
    }
    while (true){
        struct sockaddr_in clientaddr;
        socklen_t clientaddrlen = sizeof(clientaddr);
        int clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
        if(clientfd != -1){
            char recvBuf[32] = {0};
            int ret = recv(clientfd, recvBuf, 32, 0);
            if(ret>0){
                std::cout<<"recv data from client, data: "<<recvBuf<<std::endl;
                ret = send(clientfd, recvBuf, strlen(recvBuf), 0);
                if(ret != strlen(recvBuf)) std::cout<<"send error"<<std::endl;
                else std::cout<<"send successful, data: "<<recvBuf<<std::endl;
            }
            else std::cout<<"recv data error"<<std::endl;
            while (true)
            {
                sleep(3);
            }
            
            close(clientfd);
        }
    }
    close(listenfd);
    return 0;
}