#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "common.h"

void Connect(int sock, sock_info* sock_details);
void Communicate(int sock);

int main(void)
{

    sock_info sock_details = {0};

    int sock = CreateSock(&sock_details, "../test.unix-socket");

    Connect(sock, &sock_details);
    Communicate(sock);

    return 0;
}

void Connect(int sock, sock_info* sock_details){

    if(connect(sock, (struct sockaddr*)sock_details, sizeof(sock_info)) == -1)
    {
        perror("connect");
        printf("サーバーに接続できませんでした\n");
        close(sock);       
    }
}

void SendToServer(int sock){

    char buff_send[MAX_BUFF_SIZE];
    scanf("%s", buff_send);
    write(sock, buff_send, sizeof(buff_send));
    
}

void ReceiveFromServer(int sock){
    char buff_read[MAX_BUFF_SIZE];
    read(sock, buff_read, sizeof(buff_read));
    printf("%s\n", buff_read);
}

void Communicate(int sock){

    while(1){
        SendToServer(sock);
        ReceiveFromServer(sock);

        SendToServer(sock);
        ReceiveFromServer(sock);

        SendToServer(sock);
        ReceiveFromServer(sock);
    }
}