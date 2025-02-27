#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>
#include<string.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 3000
#define SEND_DATA "helloworld"

int main(int argc, char* argv[]){
    //1.创建socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd==-1){
        std::cout<<"create client failed"<<std::endl;
        return -1;
    }

    //为客户端绑定端口
    struct sockaddr_in clientaddr;
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientaddr.sin_port = htons(20000);
    if(bind(clientfd, (struct sockaddr*)&clientaddr, sizeof(clientaddr)) == -1){
        std::cout<<"client bind failed"<<std::endl;
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);
    if(connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))==-1){
        std::cout<<"connect failed"<<std::endl;
        return -1;
    }

    //3.向服务器发送数据
    int ret = send(clientfd, SEND_DATA, strlen(SEND_DATA), 0);
    if(ret != strlen(SEND_DATA)){
        std::cout<<"send error"<<std::endl;
        return -1;
    }
    std::cout<<"send successfully"<<std::endl;

    char recvBuf[32] = {0};
    ret = recv(clientfd, recvBuf, 32, 0);
    if(ret > 0) std::cout<<"receive data from server, data: "<<recvBuf<<std::endl;
    else std::cout<<"recv data error, data: "<<recvBuf<<std::endl;

    while(true) sleep(3);
    close(clientfd);
    return 0;
}