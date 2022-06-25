#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "common.h"
#include "common_client.h"

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

    char buff_input[MAX_BUFF_SIZE];
    char buff_send[MAX_BUFF_SIZE];
    memset(buff_input, 0, sizeof(buff_input));
    memset(buff_send, 0, sizeof(buff_send));
    fflush(stdin);
    scanf("%s", buff_input);
    
    //更新要求
    if(buff_input[0] == '2'){
        buff_send[0] = 2;
        buff_send[1] = 'X';
        buff_send[2] = 'k';
        buff_send[3] = 'a';
        buff_send[4] = 'n';
        buff_send[5] = 'e';
    }
    //閲覧要求
    else if(buff_input[0] == '3'){
        buff_send[0] = 3;
        if(buff_input[1] == '0'){
            buff_send[1] = 0;
        }
        else if(buff_input[1] == '1'){
            buff_send[1] = 1;
        }
        else if(buff_input[1] == '2'){
            buff_send[1] = 2;
        }
    }
    //ログアウト
    else if(buff_input[0] == '4'){
        printf("logout request");
        buff_send[0] = 4;
        buff_send[1] = 'X';
    }
    //ログイン
    else if(buff_input[0] == '5'){
        buff_send[0] = 5;
        buff_send[1] = 'X';
        //GID
        buff_send[2] = 'G';
        buff_send[3] = 'I';
        buff_send[4] = 'D';
        buff_send[5] = '1';
        buff_send[6] = '2';
        buff_send[7] = '3';
        buff_send[8] = '4';
        buff_send[9] = '5';
        buff_send[10] = '6';
        buff_send[11] = '7';
        //PIN
        buff_send[12] = 'P';
        buff_send[13] = 'I';
        buff_send[14] = 'N';
        buff_send[15] = 'L';
    }

    write(sock, buff_send, sizeof(buff_send));
    
}

void ReceiveFromServer(int sock){
    char buff_read[MAX_BUFF_SIZE];
    read(sock, buff_read, sizeof(buff_read));
    printf("文字列");
    printf("%s\n", buff_read);
    printf("数字");
    printf("%d\n", buff_read[0]);
    printf("%d\n", buff_read[1]);
}

void Communicate(int sock){

    while(1){
        SendToServer(sock);
        ReceiveFromServer(sock);

 
    }
}