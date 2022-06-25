#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include "common.h"



int Accept(int sock, sock_info* sock_details){
    int size_sock_details = sizeof(sock_info);
    int sock_client = accept(sock, (struct sockaddr*)&sock_details, (socklen_t *)&size_sock_details);
    return sock_client;
}


void ReadRequest(char* destination, char* source, int length, int offset){
        for(int i = 0; i < length; i++){
            destination[i] = source[i + offset];
        }
        //destination[length] = NULL;
}

int CanIRecv(int fd){ 
	fd_set fdset;
	struct timeval timeout;
	FD_ZERO( &fdset ); 
	FD_SET( fd , &fdset );
	timeout.tv_sec = 0; 
	timeout.tv_usec = 0;
	return select( fd+1 , &fdset , NULL , NULL , &timeout );
}


bool NeedExit(){

        char read_buff[20];
        memset(read_buff, '\0', 10);
        int val2 = 1;
        ioctl(0, FIONBIO, &val2);
        //標準入力バッファに格納されたキーボード入力を読み取る
        read(0,read_buff, 10);
        //printf("333");
        //printf("%s\n", read_buff);

        if(strcmp(read_buff, "a\n") == 0){
            return true;
        }
        return false;
}


void Communicate(int sock_client){


    while(1){
        char buff_read[MAX_BUFF_SIZE];
        char header[5];
        
        char body[16];

        read(sock_client, buff_read, sizeof(buff_read));
        ReadRequest(header, buff_read, 5, 0);

        if(NeedExit()){
            break;
        }

        
        //printf("%s\n", buff_read);

        if(strcmp(buff_read, "END") == 0){
            close(sock_client);
            break;
        }

        else{
            char buff_send[MAX_BUFF_SIZE] = "OK";
            write(sock_client, buff_send, sizeof(buff_send));
        }
    }
}





int main(void)
{

    
    sock_info sock_details = {0};
    
    int sock = BindedSocket(&sock_details, "../test.unix-socket");

 


    
    printf("escキーでサーバープログラムを終了できます");
    while(1){
        int val3 = 1;
        ioctl(sock, FIONBIO, &val3);

        int val2 = 1;
        ioctl(0, FIONBIO, &val2);    

        listen(sock, 5);
        int sock_client = Accept(sock, &sock_details);
        int val = 1;
        ioctl(sock_client, FIONBIO, &val);
        if(sock_client >= 0){
            Communicate(sock_client);
        }

        if(NeedExit()){
            break;
        }
        
               
    }
     

    return 0;


}

