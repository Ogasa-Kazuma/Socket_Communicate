#ifndef __PROGRAM1_H__
#define __PROGRAM1_H__


#include <stdlib.h>
#include <string>

using namespace std;

typedef struct sockaddr_un sock_info;

int CreateSock(sock_info* sock_details, const char* path){

    int sock = 0;

    if((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
        perror("socket");
        return -1;
    }

    sock_details -> sun_family = AF_UNIX;
    strcpy(sock_details -> sun_path, path);

    return sock;
}


int Bind(int sock, sock_info* sock_details){

    remove(sock_details -> sun_path);

    if(bind(sock, (struct sockaddr*)sock_details, sizeof(sock_info)) == -1){
        perror("bind");
        close(sock);
        exit(1);
    }

    return sock;
}


int BindedSocket(sock_info* sock_details, const char* path){
    //const char*にしたのがC++での変更点
    int sock = CreateSock(sock_details, path);
    sock = Bind(sock, sock_details);
    return sock;

}

#endif