#include "ClientManager.h"



ClientManager :: ClientManager(){

}

void ClientManager :: ReadRequest(char* destination, char* source, int length, int offset){
        for(int i = 0; i < length; i++){
            destination[i] = source[i + offset];
        }
        //destination[length] = NULL;
}


void ClientManager :: LoginResponse(char* request, char* response, char* gid_logined_user){
    char gid[size_gid];
    char pin[size_pin];

    for(int i = 0; i< sizeof(gid); i++){
        gid[i] = request[i + size_header];
    }
    for(int j = 0; j < sizeof(pin); j++){
        pin[j] = request[j + size_header + sizeof(gid)];
    }

    bool canLogin = true;
    bool canUpdate = true;

    if(canLogin){
        if(canUpdate){
            response[0] = 1;
            response[1] = 0;
            
        }
        else{
            response[0] = 1;
            response[1] = 1;            
        }
        strncpy(gid_logined_user, gid, sizeof(gid));

    }
    else if(canLogin == false){
        if(canUpdate){
            response[0] = 0;
            response[1] = 0;
        }
        else{
            response[0] = 0;
            response[1] = 1;            
        }     
    }
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