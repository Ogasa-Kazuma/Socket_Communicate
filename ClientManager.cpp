#include "ClientManager.h"



ClientManager :: ClientManager(){

}

void ClientManager :: ReadRequest(char* destination, char* source, int length, int offset){
        for(int i = 0; i < length; i++){
            destination[i] = source[i + offset];
        }
        //destination[length] = NULL;
}

void ClientManager :: MakeResponse(char* request, char* response){
    char header[size_header]; 
    char body[size_body];

    //ヘッダ情報の読み取り
    this -> ReadRequest(header, request, 5, 0);
    if(strcmp(header, "upd") == 0){
        //this -> dataManager -> Update("type", "value");
        //const char write_buff[100] = "データ更新\n";
        //write(1, write_buff, 100);
        //exit(0);
    }
    if(strcmp(header, "out") == 0){
        //const char write_buff[100] = "ログアウト\n";
        //write(1, write_buff, 100);
    }

    strcpy(response, "MakeResponse, but not send yet");

}

void ClientManager :: Syslog(const char* message){
    syslog(LOG_INFO, "%s", message);
}